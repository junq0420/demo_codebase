#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "../inc/memory_management.h"


static size_t currentMemoryUsage = 0;

// safe allocate memory
void *SafeMalloc(size_t size)
{
	void *ptr = malloc(size);
	if (ptr) {
		currentMemoryUsage += size;
	} else {
		fprintf(stderr, "Error: Memory allocation failed for %zu bytes.\n", size);
		exit(EXIT_FAILURE);
	}
	return ptr;
}

// safe reallocate memory
void *SafeRealloc(void *ptr, size_t newSize)
{
	size_t oldSize = malloc_usable_size(ptr);
	void *newPtr = realloc(ptr, newSize);
	if (newPtr) {
		currentMemoryUsage += newSize - oldSize;
	} else {
		fprintf(stderr, "Error: Memory reallocation failed for %zu bytes.\n", newSize);
		exit(EXIT_FAILURE);
	}
	return newPtr;
}

// safe free memory
void SafeFree(void **ptr)
{
	if (ptr && *ptr) {
		currentMemoryUsage -= malloc_usable_size(*ptr);
		free(*ptr);
		*ptr = NULL;
	}
}

// print current memory usage
void PrintMemoryUsage(void)
{
	printf("Current memory usage: %zu bytes.\n", currentMemoryUsage);
}

// write memory usage to file
void MemoryManagement_DumpToFile(const char *filename)
{
	FILE *fp = NULL;
	fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "Error: Unable to open %s file for writing.\n", filename);
		return ;
	}

	fprintf(fp, "Current memory usage: %zu bytes.\n", currentMemoryUsage);

	fclose(fp);
}
