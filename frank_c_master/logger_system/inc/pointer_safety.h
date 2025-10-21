#ifndef __POINTER_SAFETY_H__
#define __POINTER_SAFETY_H__

#include <stdbool.h>
#include "error_handle.h"

bool is_nullptr(const void *ptr);

/*#define SAFE_DEREF(ptr, defaultValue)\*/
/*	(is_nullptr(ptr) ? \*/
/*	(log_error("Attempted to dereference a NULL pointer."), \*/
/*	defaultValue) : *(ptr));*/

#define SAFE_DEREF(ptr, def)\
	((ptr) != NULL ? (*(ptr)) : (def))


#endif // __POINTER_SAFETY_H__
