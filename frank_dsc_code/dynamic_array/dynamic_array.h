#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

#include <stddef.h>

/* design different data struct for user */
// typedef int Data;
typedef struct {
	int age;
	char name[20];
} Data;

typedef struct {
	Data *data;
	size_t size;
	size_t capacity;
} DynamicArray;

// create a new dynamic array
DynamicArray *create_array(size_t init_capacity);

// destroy dynamic array
void destroy_array(DynamicArray *arr);

// append new data to array
int array_append(DynamicArray *arr, Data data);

// insert new data to array by index, and allow to insert at ending of the array
int array_insert(DynamicArray *arr, size_t index, Data data);

// delete data from array by index
int array_delete(DynamicArray *arr, size_t index);

// update data in array by index
int array_update(DynamicArray *arr, size_t index, Data data);

// get data from array by index
Data *array_read(DynamicArray *arr, size_t index);

// print array for debugging and support custom print data function
//void print_array(DynamicArray *arr);
void print_array(DynamicArray *arr, void (*print_func)(const Data *data));

#endif // !__DYNAMIC_ARRAY_H__
