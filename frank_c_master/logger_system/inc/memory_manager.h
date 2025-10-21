#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

/**
 * safely allocate memory and record log errors if allocation fails
 * @param size the size of allocated memory
 * @retval returns a pointer to allocated memory, or NULL on failure
 *
**/
void *safe_malloc(size_t size);


/**
 * safely reallocate memory and record log errors if allocation fails
 * @param ptr the original pointer
 * @param newSize the new memory size
 * @retval returns a pointer to the new allocated memory, or NULL on failure
 *
**/
void *safe_remalloc(void *ptr, size_t newSize);


/**
 * safely free allocated memory
 * @param ptr the address of pointer that point the allocated memory
 *
**/
void safe_free(void **ptr);

#endif // __MEMORY_MANAGER_H__

