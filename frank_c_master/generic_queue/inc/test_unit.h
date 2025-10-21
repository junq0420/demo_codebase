#ifndef __TEST_UNIT_H__
#define __TEST_UNIT_H__

#include <stdlib.h>

typedef enum {
	TEST_PASSED,
	TEST_FAILED,
} TestResult;

typedef TestResult(*TestCaseFunction)(void);

typedef struct {
	char *testName;
	TestCaseFunction function;
} TestCase;

#define VERIFY(expression) \
	do { \
		if (!(expression)) { \
			printf("Test failed: %s, Line: %d\n", __FILE__, __LINE__); \
			return TEST_FAILED; \
		} \
	} while (0)


void RunTestSuite(TestCase *tests, size_t testCount);

TestResult Test_GenericQueue_Enqueue_Dequeue(void);
TestResult Test_MemoryManagement_Allocation_Free(void);
TestResult Test_ErrorHandling(void);

#endif // __TEST_UNIT_H__
