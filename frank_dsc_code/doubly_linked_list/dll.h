#ifndef __DLL_H__
#define __DLL_H__

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char *title;
    char *artist;
    int duration;
} Song;

typedef struct Node {
    Song data;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    size_t size;
} DoublyLinkedList;

DoublyLinkedList *createList();

void freeList(DoublyLinkedList *list);

bool appendList(DoublyLinkedList *list, Song newSong);

bool prependList(DoublyLinkedList *list, Song newSong);

bool insertAfter(DoublyLinkedList *list, Node *targetNode, Song newSong);

// nodeToDelete 是要删除的节点指针
bool deleteNode(DoublyLinkedList *list, Node *nodeToDelete);

Node *findByTitle(const DoublyLinkedList *list, const char *title);

void printListForward(const DoublyLinkedList *list);

void printListBackward(const DoublyLinkedList *list);



#endif// __DLL_H__