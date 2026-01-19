#include <stdio.h>      // for perror
#include <stdlib.h>     // for malloc
#include <string.h>     // for memcpy
#include <pthread.h>    // for pthread functions

#include "linked_stack.h"

// --- Private Structure Definition ---
struct Node {
    void *data;
    struct Node *next;
};

typedef struct MemoryPool {
    Node *memory_pool;
    Node *idle_list_head;
} MemoryPool;

struct Stack {
	size_t element_size;// 栈元素数据大小
	size_t size;// 栈元素数量
	Node *top;  // 栈顶节点指针
    MemoryPool *memory_pool;
    pthread_mutex_t lock;
    FreeFunc user_free_func;
};

// --- Private Functions ---
static MemoryPool *memory_pool_create(size_t capacity)
{ 
    if (capacity == 0) {
        perror("Invalid capacity");
        return NULL;
    }

    // 1. 分配内存池结构体
    MemoryPool *pool = malloc(sizeof(MemoryPool));
    if (pool == NULL) {
        perror("Failed to allocate memory for memory pool");
        return NULL;
    }

    // 2. 分配内存池管理的节点内存
    pool->memory_pool = malloc(sizeof(Node) * capacity);
    if (pool->memory_pool == NULL) {
        perror("Failed to allocate memory for memory pool");
        free(pool);
        return NULL;
    }

    // 3. 将内存节点串成链表
    for (size_t i = 0; i < capacity - 1; i++) {
        pool->memory_pool[i].next = &pool->memory_pool[i + 1];
    }
    pool->memory_pool[capacity - 1].next = NULL;

    // 4. 初始化空闲链表头节点
    pool->idle_list_head = pool->memory_pool;

    return pool;
}

static void memory_pool_destroy(MemoryPool **p_pool)
{
    if (p_pool == NULL || *p_pool == NULL) {
        perror("Invalid p_pool");
        return;
    }

    free((*p_pool)->memory_pool);
    free(*p_pool);
    *p_pool = NULL;
}

static Node *memory_pool_alloc(MemoryPool *pool)
{
    if (pool == NULL) {
        perror("Invalid memory_pool");
        return NULL;
    }

    Node *node = pool->idle_list_head;
    pool->idle_list_head = node->next;

    return node;
}

static void memory_pool_free(MemoryPool *pool, Node *node)
{
    if (pool == NULL || node == NULL) {
        perror("Invalid pool or node");
        return;
    }

    node->next = pool->idle_list_head;
    pool->idle_list_head = node;
}

// --- Public API Implementations ---

Stack *stack_create(size_t memory_pool_capacity, size_t element_size, FreeFunc free_func)
{
    if (memory_pool_capacity == 0 || element_size == 0) {
        perror("Invalid memory_pool_capacity or element_size");
        return NULL;
    }
    
    // 1. 创建栈结构体
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        perror("Failed to allocate memory for stack");
        return NULL;
    }

    // 2. 创建内存池
    stack->memory_pool = memory_pool_create(memory_pool_capacity);
    if (stack->memory_pool == NULL) {
        perror("Failed to create memory pool");
        return NULL;
    }

    // 3. 初始化线程锁
    if (pthread_mutex_init(&stack->lock, NULL) != 0) {
        perror("Failed to initialize thread lock");
        memory_pool_destroy(&stack->memory_pool);
        free(stack);
        return NULL;
    }

    stack->top = NULL;
    stack->size = 0;
    stack->element_size = element_size;
    stack->user_free_func = free_func;

    return stack;
}

void stack_destroy(Stack **p_stack)
{
    if (p_stack == NULL || *p_stack == NULL) {
        perror("Invalid p_stack");
        return;
    }

    Stack *stack = *p_stack;

    pthread_mutex_lock(&stack->lock);

    // 1. 先释放栈中的元素数据
    for (Node *current = stack->top; current != NULL; current = current->next) {
        if (stack->user_free_func && current->data)
            stack->user_free_func(current->data);
    }
    // 2. 销毁内存池
    memory_pool_destroy(&stack->memory_pool);

    // 3. 销毁线程锁
    pthread_mutex_unlock(&stack->lock);
    pthread_mutex_destroy(&stack->lock);

    free(stack);
    *p_stack = NULL;
}

bool stack_push(Stack *stack, void *element_data)
{
    if (stack == NULL || element_data == NULL) {
        perror("Invalid stack or element_data");
        return false;
    }

    pthread_mutex_lock(&stack->lock);

    // 1. 从内存池中分配一个节点
    Node *new_node = memory_pool_alloc(stack->memory_pool);
    if (new_node == NULL) {
        perror("Failed to allocate memory for new node");
        pthread_mutex_unlock(&stack->lock);
        return false;
    }

    // 2. 为节点分配数据内存
    new_node->data = malloc(stack->element_size);
    if (new_node->data == NULL) {
        perror("Failed to allocate memory for node data");
        memory_pool_free(stack->memory_pool, new_node);
        pthread_mutex_unlock(&stack->lock);
        return false;
    }
    
    // 3. 将数据复制到新节点中
    memcpy(new_node->data, element_data, stack->element_size);

    new_node->next = stack->top;
    stack->top = new_node;

    stack->size++;
    pthread_mutex_unlock(&stack->lock);
    return true;
}

bool stack_pop(Stack *stack, void *output_buffer)
{
    if (stack == NULL || output_buffer == NULL || stack_is_empty(stack)) {
        perror("Invalid stack, output_buffer or stack is empty");
        return false;
    }

    pthread_mutex_lock(&stack->lock);

    // 获取栈顶元素
    Node *node_to_pop = stack->top;
    stack->top = node_to_pop->next;// 栈顶元素出栈
    memcpy(output_buffer, node_to_pop->data, stack->element_size);// 将栈顶元素数据拷贝到输出缓冲区
    memory_pool_free(stack->memory_pool, node_to_pop);// 将栈顶元素放回内存池

    stack->size--;
    pthread_mutex_unlock(&stack->lock);
    return true;
}

bool stack_peek(Stack *stack, void *output_buffer)
{
    if (stack == NULL || output_buffer == NULL || stack_is_empty(stack)) {
        perror("Invalid stack, output_buffer or stack is empty");
        return false;
    }

    pthread_mutex_lock(&stack->lock);

    memcpy(output_buffer, stack->top->data, stack->element_size);// 将栈顶元素数据拷贝到输出缓冲区
    pthread_mutex_unlock(&stack->lock);
    return true;
}

bool stack_is_empty(Stack *stack)
{
    if (stack == NULL) {
        perror("Invalid stack");
        return false;
    }

    return stack->top == NULL;
}

size_t stack_get_size(Stack *stack)
{
    if (stack == NULL) {
        perror("Invalid stack");
        return false;
    }

    return stack->size;
}

void stack_foreach(Stack *stack, ActionFunc action_func)
{
    if (stack == NULL || action_func == NULL || stack_is_empty(stack)) {
        perror("Invalid stack, action_func or stack is empty");
        return;
    }

    pthread_mutex_lock(&stack->lock);

    for (Node *current = stack->top; current != NULL; current = current->next) {
        if (action_func && current->data)
            action_func(current->data);
    }

    pthread_mutex_unlock(&stack->lock);
}