#include <stdio.h> 		// for perror
#include <stdlib.h> 	// for malloc, free
#include <string.h> 	// for memcpy

#include "sequential_stack.h"

// --- Private Structure Definition ---
struct Stack {
	void *data;
	size_t capacity;
	size_t element_size;
	int top; // 栈顶索引，-1 表示空栈
	PrintFunc print_func;
};

// --- Public API Implementations ---

Stack *stack_create(size_t capacity, size_t element_size, PrintFunc print_func)
{
	if (capacity == 0 || element_size == 0 || print_func == NULL) {
		perror("Invalid capacity, element_size or print_func");
		return NULL;
	}

	// 1. 分配栈结构体内存
	Stack *stack = malloc(sizeof(Stack));
	if (stack == NULL) {
		perror("Failed to allocate memory for stack");
		return NULL;
	}

	// 2. 分配栈管理的数组内存
	stack->data = malloc(capacity * element_size);
	if (stack->data == NULL) {
		perror("Failed to allocate memory for stack->data");
		free(stack);
		return NULL;
	}

	// 3. 初始化栈结构体
	stack->capacity = capacity;
	stack->element_size = element_size;
	stack->print_func = print_func;
	stack->top = -1; // 空栈初始为 -1

	return stack;
}

void stack_destroy(Stack **p_stack)
{
	if (p_stack == NULL || *p_stack == NULL) {
		perror("Invalid p_stack");
		return;
	}

	free((*p_stack)->data);
	free(*p_stack);
	*p_stack = NULL;
}

bool stack_push(Stack *stack, void *element_data)
{
	if (stack == NULL || element_data == NULL) {
		perror("Invalid stack or element_data");
		return false;
	}

	// 1. 先检查栈是否已满
	if (stack_is_full(stack)) {
		perror("Now stack is full, dont push any element");
		return false;
	}

	// 2. 栈顶偏移
	stack->top++;

	// 3. 由于使用 void* 作通用数据类型，没法确定使用时具体传入的元素大小。于是通过基地址+偏移计算当前栈顶的物理地址
	// char* 类型强转使得能够以1字节大小进行偏移
	void *dest_address = (char *)stack->data + (stack->top * stack->element_size);

	// 4. 新元素数据拷贝到栈顶
	memcpy(dest_address, element_data, stack->element_size);

	return true;
}

bool stack_pop(Stack *stack, void *output_buffer)
{
	if (stack == NULL || output_buffer == NULL) {
		perror("Invalid stack or output_buffer");
		return false;
	}

	// 1. 先判断栈是否已空
	if (stack_is_empty(stack)) {
		perror("Now stack is empty, dont pop any element");
		return false;
	}

	// 2. 计算栈顶物理地址
	void *src_address = (char *)stack->data + (stack->top * stack->element_size);

	// 3. 栈顶元素数据拷贝到 output_buffer
	memcpy(output_buffer, src_address, stack->element_size);

	// 4. 更新栈顶索引，逻辑删除，待下次 push 时会覆盖之前的数据，不需处理
	stack->top--;

	return true;
}

bool stack_peek(Stack *stack, void *output_buffer)
{
	if (stack == NULL || output_buffer == NULL) {
		perror("Invalid stack or output_buffer");
		return false;
	}

	// 1. 先判断栈是否已空
	if (stack_is_empty(stack)) {
		perror("Now stack is empty, dont peek any element");
		return false;
	}

	// 2. 计算栈顶物理地址
	void *src_address = (char *)stack->data + (stack->top * stack->element_size);

	// 3. 栈顶元素数据拷贝到 output_buffer
	memcpy(output_buffer, src_address, stack->element_size);

	// 相比与 pop 不需要把栈顶元素删除
	// 4. 更新栈顶索引
	// stack->top--;

	return true;
}

bool stack_is_empty(Stack *stack)
{
	if (stack == NULL) {
		perror("Invalid stack");
		return false;
	}

	return stack->top == -1;
}

bool stack_is_full(Stack *stack)
{
	if (stack == NULL) {
		perror("Invalid stack");
		return false;
	}

	return stack->top + 1 == stack->capacity;
}

size_t stack_get_size(Stack *stack)
{
	if (stack == NULL) {
		perror("Invalid stack");
		return false;
	}

	return (size_t)stack->top + 1;
}

size_t stack_get_capacity(Stack *stack)
{
	if (stack == NULL) {
		perror("Invalid stack");
		return false;
	}

	return stack->capacity;
}


