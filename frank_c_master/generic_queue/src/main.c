#include "../inc/generic_queue.h"
#include "../inc/test_unit.h"
#include <stdio.h>


// using TestUnit to test GenericQueue Functions
TestCase tests[] = {
	{"Test_GenericQueue_Enqueue_Dequeue", Test_GenericQueue_Enqueue_Dequeue},
};

int main(void)
{
	puts("Running test suite...\n");

	size_t testCount = sizeof(tests) / sizeof(tests[0]);

	RunTestSuite(tests, testCount);

	return EXIT_SUCCESS;
}


// int main(void)
// {
// 	int testData = 123;
// 	GenericValue testValue;
// 	testValue.ptrValue = &testData;
//
// 	GenericQueue *queue = Queue_Create();
//
// 	Queue_Enqueue(queue, TYPE_I32, (GenericValue)123);
// 	Queue_Enqueue(queue, TYPE_U32, (GenericValue)456);
// 	Queue_Enqueue(queue, TYPE_I64, (GenericValue)10000L);
// 	Queue_Enqueue(queue, TYPE_U64, (GenericValue)11111L);
// 	Queue_Enqueue(queue, TYPE_F32, (GenericValue)3.14f);
// 	Queue_Enqueue(queue, TYPE_F64, (GenericValue)3.1415926);
// 	Queue_Enqueue(queue, TYPE_CHAR, (GenericValue)'X');
// 	Queue_Enqueue(queue, TYPE_PTR, testValue);
//
// 	Queue_Print(queue);
//
// 	return 0;
// }
