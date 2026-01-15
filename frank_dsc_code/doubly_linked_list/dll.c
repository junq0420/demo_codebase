#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dll.h"

static Node *createNode(Song data)
{
    // 1. 分配新节点内存
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Failed to allocate memory for newNode");
        return NULL;
    }

    // 2. 分配新节点数据内存
    newNode->data.title = (char *)malloc(strlen(data.title) + 1);
    newNode->data.artist = (char *)malloc(strlen(data.artist) + 1);
    if (newNode->data.title == NULL || newNode->data.artist == NULL) {
        perror("Failed to allocate memory for data.title or data.artist");
        free(newNode->data.title);
        free(newNode->data.artist);
        free(newNode);
        return NULL;
    }

    // 3. 初始化新节点
    strcpy(newNode->data.title, data.title);
    strcpy(newNode->data.artist, data.artist);
    newNode->data.duration = data.duration;

    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

DoublyLinkedList *createList()
{
    DoublyLinkedList *list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
    if (list == NULL) {
        perror("Failed to allocate memory for list");
        return NULL;
    }

    // 初始化 list
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

void freeList(DoublyLinkedList *list)
{
    if (list != NULL) {
        Node *current = list->head;
        Node *temp;
        while (current != NULL) {// 循环遍历节点并释放
            temp = current;
            current = current->next;
            free(temp->data.title);
            free(temp->data.artist);
            free(temp);
        }

        free(list);
    }
}

bool appendList(DoublyLinkedList *list, Song newSong)
{
    if (list == NULL) {
        perror("The list is NULL, dont to append");
        return false;
    }

    // 1. 根据数据创建新节点
    Node *newNode = createNode(newSong);
    if (newNode == NULL) {
        perror("Failed to create newNode in appendList()");
        return false;
    }

    // 2. list 尾部插入新节点
    if (list->tail == NULL) {// 空链表，头尾指针直接指向新节点
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->prev = list->tail;// 新节点 prev 先指向 tail
        list->tail->next = newNode;// tail 的 next 指向新节点
        list->tail = newNode;// 更新 tail
    }
    list->size++;

    return true;
}

bool prependList(DoublyLinkedList *list, Song newSong)
{
    if (list == NULL) {
        perror("The list is NULL, dont to prepend");
        return false;
    }

    // 1. 根据数据创建新节点
    Node *newNode = createNode(newSong);
    if (newNode == NULL) {
        perror("Failed to create newNode in prependList()");
        return false;
    }

    // 2. list 头部插入新节点
    if (list->head == NULL) {// 空链表
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->next = list->head;
        list->head->prev = newNode;
        list->head = newNode;
    }
    list->size++;

    return true;
}

bool insertAfter(DoublyLinkedList *list, Node *targetNode, Song newSong)
{
    if (list == NULL || targetNode == NULL) {
        perror("The list or targetNode is NULL, dont to insert");
        return false;
    }

    // 1. 创建新节点
    Node *newNode = createNode(newSong);
    if (newNode == NULL) {
        perror("Failed to create newNode in insertAfter()");
        return false;
    }

    // 2. 插入节点
    newNode->prev = targetNode;
    newNode->next = targetNode->next;
    if (targetNode->next != NULL)// 目标节点非尾节点
        targetNode->next->prev = newNode;
    else// 目标节点为尾节点，那么新节点会作为新的尾节点，需要更新 tail
        list->tail = newNode;
    targetNode->next = newNode;

    list->size++;

    return true;
}

// nodeToDelete 是要删除的节点指针
bool deleteNode(DoublyLinkedList *list, Node *nodeToDelete)
{
    if (list == NULL || nodeToDelete == NULL) {
        perror("The list or nodeToDelete is NULL, dont to delete");
        return false;
    }

    // 若删除的是头尾节点，需要更新头尾指针
    if (nodeToDelete->prev == NULL) {
        list->head = nodeToDelete->next;
    } else {
        nodeToDelete->prev->next = nodeToDelete->next;
    }

    if (nodeToDelete->next == NULL) {
        list->tail = nodeToDelete->prev;
    } else {
        nodeToDelete->next->prev = nodeToDelete->prev;
    }

    free(nodeToDelete->data.title);
    free(nodeToDelete->data.artist);
    free(nodeToDelete);

    list->size--;

    return true;
}

Node *findByTitle(const DoublyLinkedList *list, const char *title)
{
    if (list == NULL || title == NULL) {
        perror("The list or title is NULL, dont to find");
        return NULL;
    }

    Node *current = list->head;
    while (current) {
        if (strcmp(current->data.title, title) == 0) {
            return current;
        }
        current = current->next;
    }

    printf("[warning]: not found <%s> title", title);
    return NULL;
}

// 倒序打印，从后向前
void printListForward(const DoublyLinkedList *list)
{
    if (list == NULL) return;

    int idx = 0;

    printf("[Song List] Size: %zu\n", list->size);
    Node *current = list->tail;
    while (current) {
        int duration = current->data.duration;
        printf("%d. <%s>-%s %02d:%02d\n", idx + 1, current->data.title, current->data.artist, duration / 60, duration % 60);
        idx++;
        current = current->prev;
    }
}

// 正序打印，从前向后
void printListBackward(const DoublyLinkedList *list)
{
    if (list == NULL) return;

    int idx = 0;

    printf("[Song List] Size: %zu\n", list->size);
    Node *current = list->head;
    while (current) {
        int duration = current->data.duration;
        printf("%d. <%s>-%s %02d:%02d\n", idx + 1, current->data.title, current->data.artist, duration / 60, duration % 60);
        idx++;
        current = current->next;
    }
}


