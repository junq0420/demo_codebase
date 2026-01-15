#include <stdio.h> 
#include <stdlib.h> 
#include "dynamic_array.h" 

#define INITITAL_CAPACITY 10 

static int resize_array(DynamicArray *arr, size_t new_capacity)
{
	Data *new_data = (Data *)realloc(arr->data, sizeof(Data) * new_capacity);
	if (!new_data) {
		perror("Failed realloc for new_data.");
		return -1;
	}

	arr->data = new_data;

	return 0;
}

DynamicArray *create_array(size_t init_capacity)
{
	if (init_capacity == 0) {
		init_capacity = INITITAL_CAPACITY;
	}

	DynamicArray *arr = (DynamicArray *)malloc(sizeof(DynamicArray));
	if (!arr) {
		perror("Failed malloc for new dynamic_array.");
		return NULL;
	}

	arr->data = (Data *)malloc(init_capacity * sizeof(Data));
	if (!arr->data) {
		perror("Failed malloc for arr->data.");
		free(arr);
		return NULL;
	}

	arr->size = 0;
	arr->capacity = init_capacity;

	return arr;
}

// destroy dynamic array
void destroy_array(DynamicArray *arr)
{
	if (arr) {
		free(arr->data);
		free(arr);
	}
}

// append new data to array
int array_append(DynamicArray *arr, Data data)
{
	if (!arr) return -1;

	if (arr->size >= arr->capacity) {// need to expand array
		arr->capacity *= 2;
		resize_array(arr, arr->capacity);
	}

	arr->data[arr->size] = data;
	arr->size++;

	return 0;
}

// insert new data to array by index, and allow to insert at ending of the array
int array_insert(DynamicArray *arr, size_t index, Data data)
{
	if (!arr || index > arr->size) return -1;

	if (arr->size >= arr->capacity) {// Is expand array
		arr->capacity *= 2;
		resize_array(arr, arr->capacity);
	}

	// index = 1, [0, 1, 2, _] -> [0, 1, 2, 2] -> [0, 1, 1, 2] -> [0, n, 1, 2]
	for (size_t i = arr->size; i > index; --i) {
		arr->data[i] = arr->data[i - 1];
	}
	arr->data[index] = data;
	arr->size++;

	return 0;
}

// delete data from array by index
int array_delete(DynamicArray *arr, size_t index)
{
	if (!arr || index >= arr->size) return -1;

	// index=1, [0, 1, 2, 3] -> [0, 2, 2, 3] -> [0, 2, 3, 3]
	for (size_t i = index; i < arr->size - 1; ++i) {
		arr->data[i] = arr->data[i + 1];
	}
	arr->size--;

	if (arr->size * 4 < arr->capacity) {// neet to reduce array
		arr->capacity /= 2;
		resize_array(arr, arr->capacity);
	}

	return 0;
}

// update data in array by index
int array_update(DynamicArray *arr, size_t index, Data data)
{
	if (!arr || index >= arr->size) return -1;

	arr->data[index] = data;

	return 0;
}

// get data from array by index
Data *array_read(DynamicArray *arr, size_t index)
{
	if (!arr || index >= arr->size) return NULL;

	return &arr->data[index];
}

// print array for debugging
void print_array(DynamicArray *arr, void (*print_func)(const Data *data))
{
	printf("Array (size: %zu | capacity: %zu): [ ", arr->size, arr->capacity);
	for (size_t i = 0; i < arr->size; ++i) {
		print_func(&arr->data[i]);
	}
	printf("]\n");
}

