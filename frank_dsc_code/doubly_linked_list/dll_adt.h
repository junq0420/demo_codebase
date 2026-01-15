#ifndef __DLL_ADT_H__
#define __DLL_ADT_H__

#include <stddef.h>
#include <stdbool.h>

// Opaque pointer type - 用户只能看到定义的数据类型指针，无法直接访问其内部结构 OOP
typedef struct DoublyLinkedList DoublyLinkedList;
typedef struct DListNode DListNode;


// Callback pointer function typedefs 定义用户需要提供的数据类型，提高灵活性
typedef int (*CompareFunc)(const void *data1, const void *data2);
typedef void (*PrintFunc)(const void *data);
typedef void (*FreeFunc)(void *data);
typedef void (*ActionFunc)(void *data, void *context);


// API functions declares
// initialCapacity 初始化内存池大小
DoublyLinkedList *createList(size_t initialCapacity, FreeFunc freeFunc);

// 二级指针便于释放内存后让 list 指向 NULL，避免悬挂指针
void destroyList(DoublyLinkedList **list);

bool appendList(DoublyLinkedList *list, const void *data);

bool prependList(DoublyLinkedList *list, const void *data);

bool insertAfterTargetNode(DoublyLinkedList *list, DListNode *targetNode, const void *data);

DListNode *findNodeInList(DoublyLinkedList *list, const void *dataToFind, CompareFunc cmp);

void deleteNodeFromList(DoublyLinkedList *list, DListNode *node);

void forEachList(DoublyLinkedList *list, ActionFunc actionFunc, void *context);

size_t getSizeOfList(const DoublyLinkedList *list);

void *getDataOfNode(const DListNode *node);


#endif// __DLL_ADT_H__