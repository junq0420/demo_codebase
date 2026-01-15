#ifndef __DOUBLY_CIRCLE_LIST_H__
#define __DOUBLY_CIRCLE_LIST_H__

#include <stddef.h>
#include <stdbool.h>

// --- Opaque Pointer Type ---
typedef struct CircularDoublyLinkedList CircularDoublyLinkedList;
typedef struct CDListNode CDListNode;

// Callback Function Pointer Type
typedef int (*CompareFunc)(const void *data1, const void *data2);
typedef void (*FreeFunc)(void *data);
typedef void (*ActionFunc)(void *data, void *context);

// APIs
CircularDoublyLinkedList *createCircularList(size_t initialCapacity, FreeFunc freeFunc);

void destroyCicularList(CircularDoublyLinkedList **list_ptr);

bool appendCircularList(CircularDoublyLinkedList *list, void *data);

bool prependCircularList(CircularDoublyLinkedList *list, void *data);

bool insertAfterTargetNode(CircularDoublyLinkedList *list, CDListNode *targetNode, void *data);

// nodeToDelete 是要删除的节点指针
bool deleteNodeFromCircularList(CircularDoublyLinkedList *list, CDListNode *nodeToDelete);

CDListNode *findNodeInCircularList(CircularDoublyLinkedList *list, const void *data, CompareFunc compareFunc);

// head 向右移向下个节点
void rotateForwardList(CircularDoublyLinkedList *list);

// head 向左移向下个节点
void rotateBackwardList(CircularDoublyLinkedList *list);

void forEachList(CircularDoublyLinkedList *list, ActionFunc actionFunc, void *context);

size_t getSizeOfList(const CircularDoublyLinkedList *list);

CDListNode *getHeadOfList(const CircularDoublyLinkedList *list);

void *getDataOfNode(const CDListNode *node);

#endif// __DOUBLY_CIRCLE_LIST_H__
