#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared_stack.h"

// --- Private Structure Definition ---

struct SharedStack {
    void *data;
    size_t capacity;
    size_t element_size;
    int top1;
    int top2;
};


// --- Public API Implementations ---

SharedStack *shared_stack_create(size_t capacity, size_t element_size)
{
    if (capacity == 0 || element_size == 0) {
        perror("Invalid capacity or element size");
        return NULL;
    }

    SharedStack *stack = malloc(sizeof(SharedStack));
    if (stack == NULL) {
        perror("Failed to allocate memory for stack");
        return NULL;
    }

    stack->data = malloc(capacity * element_size);
    if (stack->data == NULL) {
        perror("Failed to allocate memory for stack data");
        free(stack);
        return NULL;
    }

    stack->capacity = capacity;
    stack->element_size = element_size;
    stack->top1 = -1;
    stack->top2 = capacity;

    return stack;
}

void shared_stack_destroy(SharedStack **p_stack)
{
    if (p_stack == NULL || *p_stack == NULL) {
        perror("Invalid pointer to stack");
        return;
    }

    free((*p_stack)->data);
    free(*p_stack);
    *p_stack = NULL;
}

bool shared_stack_push(SharedStack *stack, STACK_NUM stack_num, const void *data)
{
    if (stack == NULL || data == NULL) {
        perror("Invalid stack or data pointer");
        return false;
    }

    if (shared_stack_is_full(stack)) {
        perror("Stack is full");
        return false;
    }

    char *dest_address;

    if (stack_num == STACK_ONE) {
        stack->top1++;
        dest_address = (char *)stack->data + stack->top1 * stack->element_size;
    } else {
        stack->top2--;
        dest_address = (char *)stack->data + stack->top2 * stack->element_size;
    }
    memcpy(dest_address, data, stack->element_size);

    return true;
}

bool shared_stack_pop(SharedStack *stack, STACK_NUM stack_num, void *output_buffer)
{
    if (stack == NULL || output_buffer == NULL) {
        perror("Invalid stack or output buffer pointer");
        return false;
    }

    if (shared_stack_is_empty(stack, stack_num)) {
        perror("Stack is empty");
        return false;
    }

    char *src_address;
    if (stack_num == STACK_ONE) {
        src_address = (char *)stack->data + stack->top1 * stack->element_size;
        stack->top1--;
    } else {
        src_address = (char *)stack->data + stack->top2 * stack->element_size;
        stack->top2++;
    }

    memcpy(output_buffer, src_address, stack->element_size);
    return true;
}

bool shared_stack_peek(SharedStack *stack, STACK_NUM stack_num, void *output_buffer)
{
    if (stack == NULL || output_buffer == NULL) {
        perror("Invalid stack or output buffer pointer");
        return false;
    }

    if (shared_stack_is_empty(stack, stack_num)) {
        perror("Stack is empty");
        return false;
    }

    char *src_address;
    if (stack_num == STACK_ONE) {
        src_address = (char *)stack->data + stack->top1 * stack->element_size;
    } else {
        src_address = (char *)stack->data + stack->top2 * stack->element_size;
    }

    memcpy(output_buffer, src_address, stack->element_size);
    return true;
}

bool shared_stack_is_empty(SharedStack *stack, STACK_NUM stack_num)
{
    if (stack == NULL) {
        perror("Invalid stack pointer");
        return false;
    }

    return stack_num == STACK_ONE ? stack->top1 == -1 : stack->top2 == stack->capacity;
}

bool shared_stack_is_full(SharedStack *stack)
{
    if (stack == NULL) {
        perror("Invalid stack pointer");
        return false;
    }

    return stack->top1 == stack->top2 - 1;
}

size_t shared_stack_get_size(SharedStack *stack, STACK_NUM stack_num)
{
    if (stack == NULL) {
        perror("Invalid stack pointer");
        return 0;
    }

    return stack_num == STACK_ONE ? (size_t)stack->top1 + 1 : stack->capacity - stack->top2;
}

size_t shared_stack_get_capacity(SharedStack *stack)
{
    if (stack == NULL) {
        perror("Invalid stack pointer");
        return 0;
    }

    return stack->capacity;
}
