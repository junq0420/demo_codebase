#include <stdio.h>

#include "../inc/generic_queue.h"
#include "../inc/test_unit.h"


void RunTestSuite(TestCase *tests, size_t testCount)
{
	size_t passedCount = 0;

	for (size_t i = 0; i < testCount; i++) {
		printf("Running test: %s\n", tests[i].testName);
		TestResult result = tests[i].function();
		if (result == TEST_PASSED) {
			printf("%s Test Passed\n", tests[i].testName);
			passedCount++;
		} else {
			printf("%s Test Failed\n", tests[i].testName);
		}
	}

	printf("\nTest Suite Summary: %zu/%zu tests passed\n", passedCount, testCount);
}

TestResult Test_GenericQueue_Enqueue_Dequeue(void)
{
	printf("Running Test_GenericQueue_Enqueue_Dequeue...\n");

	GenericQueue *queue = Queue_Create();
	if (!queue) {
		printf("Queue_Create failed\n");
		return TEST_FAILED;
	}

	int testData = 42;
	GenericValue value;
	value.ptrValue = &testData;

	bool enqueueResult = Queue_Enqueue(queue, TYPE_PTR, value);
	if (!enqueueResult) {
		printf("Queue_Enqueue failed\n");
		return TEST_FAILED;
	}

	GenericValue dequeueValue;
	DataType dequeueType;
	bool dequeueResult = Queue_Dequeue(queue, &dequeueType, &dequeueValue);
	if (!dequeueResult) {
		printf("Queue_Dequeue failed\n");
		return TEST_FAILED;
	}
	VERIFY(dequeueType == TYPE_PTR);
	VERIFY((*(int *)dequeueValue.ptrValue) == testData);

	Queue_Destroy(queue);
	printf("Test_GenericQueue_Enqueue_Dequeue passed\n");
	return TEST_PASSED;
}

TestResult Test_MemoryManagement_Allocation_Free(void);
TestResult Test_ErrorHandling(void);
