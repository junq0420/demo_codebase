#include <stdlib.h>
#include "datatype.h"

typedef struct GenericQueueNode {
	DataType type;
	GenericValue value;
	struct GenericQueueNode *next;
} GenericQueueNode;

typedef struct {
	GenericQueueNode *head;
	GenericQueueNode *tail;
	size_t size;
} GenericQueue;

GenericQueue *Queue_Create(void);
void Queue_Destroy(GenericQueue *queue);
bool Queue_Enqueue(GenericQueue *queue, DataType type, GenericValue value);
bool Queue_Dequeue(GenericQueue *queue, DataType *type, GenericValue *value);
size_t Queue_GetSize(GenericQueue *queue);
bool Queue_IsEmpty(GenericQueue *queue);
void Queue_Print(GenericQueue *queue);
