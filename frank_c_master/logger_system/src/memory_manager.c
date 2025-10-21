#include "../inc/memory_manager.h"
#include "../inc/logger.h"
#include <stdalign.h>


void *safe_malloc(size_t size) {
	assert(size > 0);

	void *ptr = NULL;

// safely allocate aligned memory after C11, alignof(max_align_t) is specified the alignment boundary
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
	ptr = aligned_alloc(alignof(max_align_t), size);
#else
	ptr = malloc(size);
#endif

	if (ptr == NULL) {
		// 实际应用中，可能不希望在分配失败后停止程序，这里为了调试方便找出问题使用了断言函数
		// 在实际企业开发中应该根据需求对错误进行处理
		fprintf(stderr, "Memeroy allocation failed for size %zu.\n", size);
		assert(ptr != NULL);
	}

	return ptr;
}


void *safe_remalloc(void *ptr, size_t newSize) {
	void *newPtr = realloc(ptr, newSize);
	if (!newPtr) {
		logger_msg(LOG_ERROR, "Failed to reallocate to %zu bytes of memory", newSize);
		newPtr = NULL;
	}

	return newPtr;
}


void safe_free(void **ptr) {
	if (ptr && *ptr) {
		free(*ptr);
		*ptr = NULL;
	} else {
		logger_msg(LOG_WARNING, "Attemped to free a NULL or already freed pointer");
	}
}

