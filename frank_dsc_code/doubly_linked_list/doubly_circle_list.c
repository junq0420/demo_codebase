#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "doubly_circle_list.h"

struct CDListNode {
	void *data;
	CDListNode *next;
	CDListNode *prev;
};

typedef struct MemoryPool {
	CDListNode *memoryPool;
	CDListNode *idleListHead;
} MemoryPool;

struct CircularDoublyLinkedList {
	CDListNode *head;
	size_t size;
	MemoryPool *pool;
	pthread_mutex_t lock;
	FreeFunc userFreeFunc;
};

static MemoryPool *createMemoryPool(size_t initialCapacity)
{
	if (initialCapacity == 0) {
		perror("Invalid initial capacity");
		return NULL;
	}

	// 1. 分配内存池
	MemoryPool *pool = (MemoryPool *)malloc(sizeof(MemoryPool));
	if (pool == NULL) {
		perror("Failed to allocate memory for memoryPool");
		return NULL;
	}

	// 2. 分配内存池所管理的内存块
	pool->memoryPool = (CDListNode *)malloc(sizeof(CDListNode) * initialCapacity);
	if (pool->memoryPool == NULL) {
		perror("Failed to allocate memory for pool->memoryPool");
		free(pool);
		return NULL;
	}

	// 3. 将内存块串起来
	for (size_t i = 0; i < initialCapacity - 1; i++) {
		pool->memoryPool[i].next = &pool->memoryPool[i + 1];
	}
	pool->memoryPool[initialCapacity - 1].next = NULL;

	// 4. 初始化空闲列表 head
	pool->idleListHead = pool->memoryPool;

	return pool;
}

static void destroyMemoryPool(MemoryPool **pool_ptr)
{
	if (pool_ptr == NULL || *pool_ptr == NULL) {
		perror("Invalid pool_ptr");
		return;
	}

	free((*pool_ptr)->memoryPool);
	free(*pool_ptr);
	*pool_ptr = NULL;
}

static CDListNode *allocateNodeFromPool(MemoryPool *pool)
{
	if (pool == NULL) {
		perror("pool is NULL, dont to allocate node");
		return NULL;
	}

	CDListNode *newNode = pool->idleListHead;
	pool->idleListHead = pool->idleListHead->next;

	return newNode;
}

static void freeNodeToPool(MemoryPool *pool, CDListNode *node)
{
	if (pool == NULL || node == NULL) {
		perror("Invalid pool or node");
		return;
	}

	node->next = pool->idleListHead;
	pool->idleListHead = node;
}


CircularDoublyLinkedList *createCircularList(size_t initialCapacity, FreeFunc freeFunc)
{
	if (initialCapacity == 0) {
		perror("Invalid initialCapacity");
		return NULL;
	}

	CircularDoublyLinkedList *list = (CircularDoublyLinkedList *)malloc(sizeof(CircularDoublyLinkedList));
	if (list == NULL) {
		perror("Failed to allocate memory for CircularDoublyLinkedList");
		return NULL;
	}

	list->pool = createMemoryPool(initialCapacity);
	if (list->pool == NULL) {
		perror("Failed to create list->pool");
		free(list);
		return NULL;
	}

	list->head = NULL;
	list->size = 0;
	list->userFreeFunc = freeFunc;

	if (pthread_mutex_init(&list->lock, NULL) != PTHREAD_MUTEX_NORMAL) {
		destroyMemoryPool(&list->pool);
		free(list);
		return NULL;
	}

	return list;
}

void destroyCicularList(CircularDoublyLinkedList **list_ptr)
{
	if (list_ptr == NULL || *list_ptr == NULL) {
		perror("Invalid list_ptr");
		return;
	}

	CircularDoublyLinkedList *list = *list_ptr;

	pthread_mutex_lock(&list->lock);

	// 1. 先释放循环链表每个节点的数据
	if (list->head) {
		CDListNode *current = list->head;
		for (size_t i = 0; i < list->size; i++) {
			if (list->userFreeFunc && current->data)
				list->userFreeFunc(current->data);
			current = current->next;
		}
	}

	// 2. 销毁内存池
	destroyMemoryPool(&list->pool);

	// 3. 释放并销毁锁
	pthread_mutex_unlock(&list->lock);
	pthread_mutex_destroy(&list->lock);

	free(list);
	*list_ptr = NULL;
}

bool appendCircularList(CircularDoublyLinkedList *list, void *data)
{
	if (list == NULL || data == NULL) {
		perror("Invalid list or data");
		return false;
	}

	pthread_mutex_lock(&list->lock);

	// 从内存池中分配新节点
	CDListNode *newNode = allocateNodeFromPool(list->pool);
	if (newNode == NULL) {
		perror("Failed to allocate memory for new node");
		return false;
	}
	newNode->data = data;

	if (list->head == NULL) {// list 为空，更新 head 且该节点指向自身
		list->head = newNode;
		newNode->next = newNode;
		newNode->prev = newNode;
	} else {// list 非空，将新节点与 list 的 head 和 tail 相连接
		// 在结构体定义时略去 tail，因为循环链表中可直接通过 head 找到 tail
		CDListNode *tail = list->head->prev;
		newNode->next = list->head;
		newNode->prev = tail;
		list->head->prev = newNode;
		tail->next = newNode;
	}

	list->size++;

	pthread_mutex_unlock(&list->lock);
	return true;
}

bool prependCircularList(CircularDoublyLinkedList *list, void *data)
{
	// 先将节点添加到尾部
	bool res = appendCircularList(list, data);

	// 若 list 存在多个节点则将 head 前移完成从前追加操作
	if (res && list->size > 1) {
		list->head = list->head->prev;
	}

	return res;
}

bool insertAfterTargetNode(CircularDoublyLinkedList *list, CDListNode *targetNode, void *data)
{
	if (list == NULL || list->head == NULL || targetNode == NULL || data == NULL) {
		perror("Invalid list, targetNode or data");
		return false;
	}

	// 若 targetNode 为 tail 则直接 append
	if (targetNode == list->head->prev) {
		return appendCircularList(list, data);
	}

	pthread_mutex_lock(&list->lock);

	CDListNode *newNode = allocateNodeFromPool(list->pool);
	if (newNode == NULL) {
		perror("Failed to allocate node from pool");
		return false;
	}
	newNode->data = data;

	newNode->next = targetNode->next;
	newNode->prev = targetNode;
	targetNode->next->prev = newNode;
	targetNode->next = newNode;

	list->size++;
	pthread_mutex_unlock(&list->lock);
	return true;
}

// nodeToDelete 是要删除的节点指针
bool deleteNodeFromCircularList(CircularDoublyLinkedList *list, CDListNode *nodeToDelete)
{
	if (list == NULL || list->head == NULL || nodeToDelete == NULL) {
		perror("Invalid list or nodeToDelete");
		return false;
	}

	pthread_mutex_lock(&list->lock);

	if (list->size == 1 && list->head == nodeToDelete) {
		// 若删除节点恰好为 list 唯一的 head，则清空 list
		list->head = NULL;
	} else {
		nodeToDelete->prev->next = nodeToDelete->next;
		nodeToDelete->next->prev = nodeToDelete->prev;
		if (nodeToDelete == list->head) {// 删除首节点
			list->head = nodeToDelete->next;
		}
	}


	if (list->userFreeFunc && nodeToDelete->data)
		list->userFreeFunc(nodeToDelete->data);
	freeNodeToPool(list->pool, nodeToDelete);

	list->size--;
	pthread_mutex_unlock(&list->lock);
	return true;
}

// head 向右移向下个节点
void rotateForwardList(CircularDoublyLinkedList *list)
{
	if (list == NULL) {
		perror("Invalid list");
		return;
	}

	pthread_mutex_lock(&list->lock);
	list->head = list->head->next;
	pthread_mutex_unlock(&list->lock);
}

// head 向左移向下个节点
void rotateBackwardList(CircularDoublyLinkedList *list)
{
	if (list == NULL) {
		perror("Invalid list");
		return;
	}

	pthread_mutex_lock(&list->lock);
	list->head = list->head->prev;
	pthread_mutex_unlock(&list->lock);
}

CDListNode *findNodeInCircularList(CircularDoublyLinkedList *list, const void *data, CompareFunc compareFunc)
{
	if (list == NULL || data == NULL || compareFunc == NULL) {
		perror("Invalid list, data or compareFunc");
		return NULL;
	}

	pthread_mutex_lock(&list->lock);

	CDListNode *foundNode = NULL;
	CDListNode *current = list->head;
	for (size_t i = 0; i < list->size; i++) {
		if (compareFunc(data, current->data) == 0) {
			foundNode = current;
			break;
		}
		current = current->next;
	}

	pthread_mutex_unlock(&list->lock);
	return foundNode;
}

void forEachList(CircularDoublyLinkedList *list, ActionFunc actionFunc, void *context)
{
	if (list == NULL || actionFunc == NULL) {
		perror("Invalid list or actionFunc");
		return;
	}

	pthread_mutex_lock(&list->lock);

	CDListNode *current = list->head;
	for (size_t i = 0; i < list->size; i++) {
		actionFunc(current->data, context);
		current = current->next;
	}

	pthread_mutex_unlock(&list->lock);
}

size_t getSizeOfList(const CircularDoublyLinkedList *list)
{
	return list ? list->size : 0;
}

CDListNode *getHeadOfList(const CircularDoublyLinkedList *list)
{
	return list ? list->head : NULL;
}

void *getDataOfNode(const CDListNode *node)
{
	return node ? node->data : NULL;
}

