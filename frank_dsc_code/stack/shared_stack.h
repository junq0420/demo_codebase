#ifndef __SHARED_STACK_H__
#define __SHARED_STACK_H__

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    STACK_ONE = 1,
    STACK_TWO = 2
} STACK_NUM;

typedef struct SharedStack SharedStack;


SharedStack *shared_stack_create(size_t capacity, size_t element_size);

void shared_stack_destroy(SharedStack **p_stack);

bool shared_stack_push(SharedStack *stack, STACK_NUM stack_num, const void *data);

bool shared_stack_pop(SharedStack *stack, STACK_NUM stack_num, void *output_buffer);

bool shared_stack_peek(SharedStack *stack, STACK_NUM stack_num, void *output_buffer);

bool shared_stack_is_empty(SharedStack *stack, STACK_NUM stack_num);

bool shared_stack_is_full(SharedStack *stack);

size_t shared_stack_get_size(SharedStack *stack, STACK_NUM stack_num);

size_t shared_stack_get_capacity(SharedStack *stack);


#endif //__SHARED_STACK_H__