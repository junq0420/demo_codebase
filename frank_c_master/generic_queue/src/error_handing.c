#include <stdio.h>
#include <stdlib.h>
#include "../inc/error_handing.h"

static ErrorCode currentErrorCode = ERROR_NONE;

void SetErrorCode(ErrorCode code)
{
	currentErrorCode = code;
}

ErrorCode GetErrorCode(void)
{
	return currentErrorCode;
}

const char *GetErrorString(ErrorCode code)
{
	switch (code) {
		case ERROR_NONE: return "No error.";
		case ERROR_MEMORY_ALLOCATION_FAILED: return "Memory allocation failed.";
		case ERROR_INVALID_OPERATION: return "Invalid operation.";
		case ERROR_BOUNDS: return "Bounds error.";

		default: return "Unknown error.";
	}
}

ErrorCode CheckNotNull(void *ptr)
{
	if (ptr == NULL) {
		SetErrorCode(ERROR_MEMORY_ALLOCATION_FAILED);
		return ERROR_MEMORY_ALLOCATION_FAILED;
	}

	return ERROR_NONE;
}
void HandleError(ErrorCode code)
{
	const char *errorString = GetErrorString(code);
	fprintf(stderr, "Error: %s\n", errorString);
	if (code != ERROR_NONE) {
		exit(EXIT_FAILURE);
	}
}

