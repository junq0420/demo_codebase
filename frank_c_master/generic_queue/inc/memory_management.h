#ifndef __MEMORY_MANAGEMENT_H__
#define __MEMORY_MANAGEMENT_H__

#include <stddef.h>

// safe allocate memory
void *SafeMalloc(size_t size);

// safe reallocate memory
void *SafeRealloc(void *ptr, size_t newSize);

// safe free memory
void SafeFree(void **ptr);

// print current memory usage
void PrintMemoryUsage(void);

// write memory usage to file
void MemoryManagement_DumpToFile(const char *filename);


#endif // __MEMORY_MANAGEMENT_H__
