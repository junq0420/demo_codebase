#ifndef __LINKED_STACK_H__
#define __LINKED_STACK_H__

#include <stddef.h> // for size_t
#include <stdbool.h>// for bool type

// --- Opaque Pointer Type ---
typedef struct Stack Stack;
typedef struct Node Node;

// --- Callback Function Pointer Type ---
typedef void (*ActionFunc)(void *data);
typedef void (*FreeFunc)(void *data);

// --- Public API Prototypes ---

/**
 * @brief 创建一个链式栈
 * @param memory_pool_capacity 内存池容量
 * @param element_size 栈元素数据大小
 * @param free_func 释放用户自定义函数指针
 * @return 创建的链式栈, 若 memory_pool_capacity <= 0 或者 free_func == NULL, 则返回 NULL
 */
Stack *stack_create(size_t memory_pool_capacity, size_t element_size, FreeFunc free_func);

void stack_destroy(Stack **p_stack);

bool stack_push(Stack *stack, void *element_data);

bool stack_pop(Stack *stack, void *output_buffer);

bool stack_peek(Stack *stack, void *output_buffer);

bool stack_is_empty(Stack *stack);

size_t stack_get_size(Stack *stack);

void stack_foreach(Stack *stack, ActionFunc action_func);

#endif// __LINKED_STACK_H__
