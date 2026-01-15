#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
// #include <threads.h> 
// GCC 在 Linux 上默认并不提供完整实现——头文件虽有，函数体为空，于是链接失败。改用 POSIX 线程（pthread）改用 POSIX
#include <pthread.h>
#include "dll_adt.h"

struct DListNode {
    void *data;
    DListNode *next;
    DListNode *prev;
};

// 内存池结构，提前分配一块内存池大小的内存，避免每次增删 Node 时都调用 malloc 和 free，也减少内存碎片化
typedef struct MemoryPool {
    DListNode *memoryPool;// 整块内存池的地址
    // 内存池内部通过单向链表管理空闲列表
    DListNode *idleListHead;// 内存池中空闲列表的 head
} MemoryPool;

struct DoublyLinkedList {
    DListNode *head;
    DListNode *tail;
    size_t size;
    pthread_mutex_t lock;// 互斥锁，避免多线程操作链表时出线数据竞争
    // mtx_t lock; 
    MemoryPool *pool;// 内存池
    FreeFunc userFreeFunc;// 用户自定义的释放数据内存函数
};

// 内存池相关函数
static MemoryPool *createMemoryPool(size_t initCapacity)
{
    if (initCapacity == 0) {
        perror("initial capacity is zero, dont to create memory pool");
        return NULL;
    }

    // 1. 分配内存池对象
    MemoryPool *pool = (MemoryPool *)malloc(sizeof(MemoryPool));
    if (pool == NULL) {
        perror("Failed to allocate pool");
        return NULL;
    }

    // 2. 分配内存池要管理的一块连续内存节点
    pool->memoryPool = (DListNode *)malloc(initCapacity * sizeof(DListNode));
    if (pool->memoryPool == NULL) {
        perror("Failed to allocate pool->memoryPoll");
        free(pool);
        return NULL;
    }

    // 3. 单向串起节点
    for (size_t i = 0; i < initCapacity - 1; i++) {
        pool->memoryPool[i].next = &pool->memoryPool[i + 1];
    }
    pool->memoryPool[initCapacity - 1].next = NULL;// 尾节点指向 NULL

    // 4. 初始化空闲列表 head
    pool->idleListHead = pool->memoryPool;

    return pool;
}

static void destroyMemoryPool(MemoryPool **pool)
{
    if (pool == NULL || *pool == NULL) {
        perror("pool or *pool is NULL, dont to free");
        return;
    }

    free((*pool)->memoryPool);
    free(*pool);
    *pool = NULL;
}

static DListNode *allocateNodeFromPool(MemoryPool *pool)
{
    if (pool == NULL || pool->idleListHead == NULL) {
        perror("pool is NULL or pool is empty, dont to allocate node");
        return NULL;
    }

    DListNode *newNode = pool->idleListHead;// 从空闲链表中拿取一个 Node
    pool->idleListHead = newNode->next;// 空闲链表 head 更新

    return newNode;
}

static void freeNodeToPool(MemoryPool *pool, DListNode *node)
{
    if (pool == NULL || node == NULL) {
        perror("pool or node is NULL, dont to free");
        return;
    }

    node->next = pool->idleListHead;// 将 Node 归还到空闲链表
    pool->idleListHead = node;
}


// API functions declares
// initialCapacity 初始化内存池大小
DoublyLinkedList *createList(size_t initialCapacity, FreeFunc freeFunc)
{
    if (initialCapacity == 0) {
        perror("Invalid initial capacity for doubly linked list");
        return NULL;
    }

    // 1. 分配 list 结构内存
    DoublyLinkedList *list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
    if (list == NULL) {
        perror("Failed to allocate memory for list");
        return NULL;
    }

    // 2. 初始化 list
    list->pool = createMemoryPool(initialCapacity);
    if (list->pool == NULL) {
        perror("Failed to allocate memory for list->pool");
        free(list);
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->userFreeFunc = freeFunc;

    // 初始化 Mutex
	if (pthread_mutex_init(&list->lock, NULL) != PTHREAD_MUTEX_NORMAL) {
		perror("Failed to initialize mutex");
		destroyMemoryPool(&list->pool);
		free(list);
		return NULL;
	}

    return list;
}

// 二级指针便于释放内存后让 list 指向 NULL，避免悬挂指针
void destroyList(DoublyLinkedList **list_ref)
{
    if (list_ref == NULL || *list_ref == NULL) {
        perror("list_ref or *list_ref is NULL, dont to destroy");
        return;
    }

    DoublyLinkedList *list = *list_ref;
    pthread_mutex_lock(&list->lock);

    // 遍历 list，释放每个 node 的 data 并归还给内存池
    DListNode *current = list->head;
    while (current) {
        DListNode *nextNode = current->next;
        if (list->userFreeFunc && current->data) {
            list->userFreeFunc(current->data);
        }
        current = nextNode;
    }

    destroyMemoryPool(&list->pool);

    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);// 销毁锁

    free(list);
    *list_ref = NULL;
}

static DListNode *_internalCreateNode(DoublyLinkedList *list, const void *data)
{
    if (list == NULL || data == NULL) {
        perror("list or data is NULL, dont to create node");
        return NULL;
    }

    DListNode *newNode = allocateNodeFromPool(list->pool);
    if (newNode == NULL) {
        perror("Failed to allocate node from pool");
        return NULL;
    }

    newNode->data = (void *)data;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

bool appendList(DoublyLinkedList *list, const void *data)
{
    if (list == NULL || data == NULL) {
        perror("list or data is NULL, dont to append node");
        return false;
    }

    pthread_mutex_lock(&list->lock);

    DListNode *newNode = _internalCreateNode(list, data);
    if (newNode == NULL) {
        perror("Failed to create new node");
        pthread_mutex_unlock(&list->lock);
        return false;
    }

    if (list->tail == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    
    list->size++;

    pthread_mutex_unlock(&list->lock);
    return true;
}

bool prependList(DoublyLinkedList *list, const void *data)
{
    if (list == NULL || data == NULL) {
        perror("list or data is NULL, dont to prepend node");
        return false;
    }

    pthread_mutex_lock(&list->lock);

    DListNode *newNode = _internalCreateNode(list, data);
    if (newNode == NULL) {
        perror("Failed to create new node");
        pthread_mutex_unlock(&list->lock);
        return false;
    }

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->next = list->head;
        list->head->prev = newNode;
        list->head = newNode;
    }
    
    list->size++;

    pthread_mutex_unlock(&list->lock);
    return true;
}

bool insertAfterTargetNode(DoublyLinkedList *list, DListNode *targetNode, const void *data)
{
    if (list == NULL || targetNode == NULL || data == NULL) {
        perror("Invalid list, targetNode or data");
        return false;
    }

    pthread_mutex_lock(&list->lock);

    DListNode *newNode = _internalCreateNode(list, data);
    if (newNode == NULL) {
        perror("Failed to create new node");
        pthread_mutex_unlock(&list->lock);
        return false;
    }

    newNode->prev = targetNode;
    newNode->next = targetNode->next;
    if (targetNode->next != NULL) {
        targetNode->next->prev = newNode;
    } else {
        list->tail = newNode;   
    }
    targetNode->next = newNode;

    list->size++;
    pthread_mutex_unlock(&list->lock);
    return true;
}

DListNode *findNodeInList(DoublyLinkedList *list, const void *dataToFind, CompareFunc cmp)
{
    if (list == NULL || dataToFind == NULL || cmp == NULL) {
        perror("Invalid list, dataToFind or cmp");
        return NULL;
    }

    pthread_mutex_lock(&list->lock);

    for (DListNode *current = list->head; current != NULL; current = current->next) {
        if (cmp(current->data, dataToFind) == 0) {
            pthread_mutex_unlock(&list->lock);
            return current;
        }
    }

    printf("No found with dataToFind\n");
    pthread_mutex_unlock(&list->lock);
    return NULL;
}

void deleteNodeFromList(DoublyLinkedList *list, DListNode *node)
{
    if (list == NULL || node == NULL) {
        perror("list or node is NULL, dont to delete");
        return;
    }

    pthread_mutex_lock(&list->lock);

    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }

    if (list->userFreeFunc && node->data)
        list->userFreeFunc(node->data);
    freeNodeToPool(list->pool, node);

    list->size--;
    pthread_mutex_unlock(&list->lock);
}

void forEachList(DoublyLinkedList *list, ActionFunc actionFunc, void *context)
{
    if (list == NULL || actionFunc == NULL) {
        perror("Invalid list or actionFunc");
        return;
    }

    pthread_mutex_lock(&list->lock);

    for (DListNode *current = list->head; current != NULL; current = current->next) {
        actionFunc(current->data, context);
    }

    pthread_mutex_unlock(&list->lock);
}

size_t getSizeOfList(const DoublyLinkedList *list)
{
    return list == NULL ? 0 : list->size;
}

void *getDataOfNode(const DListNode *node)
{
    return node == NULL ? 0 : node->data;
}
