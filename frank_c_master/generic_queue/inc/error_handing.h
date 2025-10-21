#ifndef __ERROR_HANDING_H__
#define __ERROR_HANDING_H__

#include <stddef.h>
#include <errno.h>

typedef enum {
	ERROR_NONE = 0,
	ERROR_MEMORY_ALLOCATION_FAILED,
	ERROR_INVALID_OPERATION,
	ERROR_BOUNDS,
} ErrorCode;

void SetErrorCode(ErrorCode code);
ErrorCode GetErrorCode(void);
const char *GetErrorString(ErrorCode code);

ErrorCode CheckNotNull(void *ptr);
void HandleError(ErrorCode code);

#endif // __ERROR_HANDING_H__
