#include "../inc/generic_queue.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


GenericQueue *Queue_Create(void)
{
	GenericQueue *queue = (GenericQueue *)malloc(sizeof(GenericQueue));
	if (queue == NULL) {
		fprintf(stderr, "malloc failed for queue: %s\n", strerror(errno));
		return NULL;
	}

	queue->head = queue->tail = NULL;
	queue->size = 0;

	return queue;
}

void Queue_Destroy(GenericQueue *queue)
{
	if (queue == NULL) return;

	GenericQueueNode *current = queue->head;
	for (size_t i = 0; i < queue->size; i++) {// free each element in queue
		GenericQueueNode *next = current->next;
		free(current);
		current = next;
	}

	free(queue);// free queue instance finally
}

bool Queue_Enqueue(GenericQueue *queue, DataType type, GenericValue value)
{
	if (queue == NULL) return false;

	GenericQueueNode *newNode = (GenericQueueNode *)malloc(sizeof(GenericQueueNode));
	if (newNode == NULL) {
		fprintf(stderr, "malloc failed for newNode: %s\n", strerror(errno));
		return false;
	}

	newNode->type = type;
	newNode->value = value;
	newNode->next = NULL;

	if (queue->tail == NULL) {// empty queue
		queue->head = queue->tail = newNode;
	} else {
		queue->tail->next = newNode;
		queue->tail = newNode;
	}

	queue->size++;

	return true;
}

bool Queue_Dequeue(GenericQueue *queue, DataType *type, GenericValue *value)
{
	if (queue == NULL || queue->head == NULL) return false;

	GenericQueueNode *nodeToRemove = queue->head;
	*type = nodeToRemove->type;
	*value = nodeToRemove->value;

	queue->head = nodeToRemove->next;
	if (queue->head == NULL) // remove the last element
		queue->tail = NULL;
	queue->size--;

	free(nodeToRemove);

	return true;
}

size_t Queue_GetSize(GenericQueue *queue)
{
	return queue ? queue->size : 0;
}

bool Queue_IsEmpty(GenericQueue *queue)
{
	return queue ? (queue->size == 0) : true;
}

static void GenericQueueNode_Print(GenericQueueNode *node)
{
	switch (node->type) {
		case TYPE_I32: printf("(Type: I32 | Val: %d)", node->value.i32Value); break;
		case TYPE_U32: printf("(Type: U32 | Val: %d)", node->value.u32Value); break;
		case TYPE_I64: printf("(Type: I64 | Val: %ld)", node->value.i64Value); break;
		case TYPE_U64: printf("(Type: U64 | Val: %ld)", node->value.u64Value); break;
		case TYPE_F32: printf("(Type: F32 | Val: %f)", node->value.f32Value); break;
		case TYPE_F64: printf("(Type: F64 | Val: %f)", node->value.f64Value); break;
		case TYPE_CHAR: printf("(Type: CHAR | Val: %c)", node->value.charValue); break;
		case TYPE_PTR: printf("(Type: PTR | Val: %p)", node->value.ptrValue); break;
		default: printf("(Unkown Type)"); break;
	}
}

void Queue_Print(GenericQueue *queue)
{
	GenericQueueNode *current = queue->head;
	int idx = 0;

	printf("Queue: \n");
	while (current != NULL) {
		printf("[%d, ", idx);
		GenericQueueNode_Print(current);
		printf("]\n");
		current = current->next;
		idx++;
	}
}
