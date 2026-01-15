#ifndef __SEQUENTIAL_STATCK_H__
#define __SEQUENTIAL_STATCK_H__

#include <stddef.h> // for size_t
#include <stdbool.h>// for bool type

// --- Opaque Pointer Type ---
typedef struct Stack Stack;

// --- Callback Function Pointer Type ---
typedef void (*PrintFunc)(const void *data);

// --- Public API Prototypes ---

/**
 * @brief 创建一个顺序栈
 * @param capacity 栈容量，可容纳最大的元素数量
 * @param element_size 栈元素大小
 * @param print_func 打印函数指针
 * @return 成功时返回所建新栈的指针，若 capacity 或 element_size 为 0
 * 		   或分配内存失败则返回 NULL
 */
Stack *stack_create(size_t capacity, size_t element_size, PrintFunc print_func);

void stack_destroy(Stack **p_stack);

bool stack_push(Stack *stack, void *element_data);

bool stack_pop(Stack *stack, void *output_buffer);

bool stack_peek(Stack *stack, void *output_buffer);

bool stack_is_empty(Stack *stack);

bool stack_is_full(Stack *stack);

size_t stack_get_size(Stack *stack);

size_t stack_get_capacity(Stack *stack);


#endif// __SEQUENTIAL_STATCK_H__
