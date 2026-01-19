#include "shared_stack.h"
#include <stdio.h>

void print_stack_status(SharedStack* stack) {
    if (!stack) {
        printf("Stack is NULL.\n");
        return;
    }
    printf("------------------------------------------\n");
    printf("Total Capacity: %zu\n", shared_stack_get_capacity(stack));
    printf("Stack 1 Size: %zu (Empty: %s)\n",
        shared_stack_get_size(stack, STACK_ONE),
        shared_stack_is_empty(stack, STACK_ONE) ? "Yes" : "No");
    printf("Stack 2 Size: %zu (Empty: %s)\n",
        shared_stack_get_size(stack, STACK_TWO),
        shared_stack_is_empty(stack, STACK_TWO) ? "Yes" : "No");
    printf("Is Full: %s\n", shared_stack_is_full(stack) ? "Yes" : "No");
    printf("------------------------------------------\n\n");
}

int main() {
    printf("--- Testing Shared Integer Stack ---\n");
    // 创建一个总容量为10的共享栈
    SharedStack *ss = shared_stack_create(10, sizeof(int));

    print_stack_status(ss);

    // 1. 向栈1压入数据
    printf("Pushing 10, 20, 30 to Stack 1...\n");
    for (int i = 10; i <= 30; i += 10) {
        shared_stack_push(ss, STACK_ONE, &i);
    }
    print_stack_status(ss);

    // 2. 向栈2压入数据
    printf("Pushing 100, 90 to Stack 2...\n");
    for (int i = 100; i >= 90; i -= 10) {
        shared_stack_push(ss, STACK_TWO, &i);
    }
    print_stack_status(ss);

    // 3. 查看栈顶元素
    int peek_val;
    if (shared_stack_peek(ss, STACK_ONE, &peek_val)) {
        printf("Peek at Stack 1 top: %d\n", peek_val);
    }
    if (shared_stack_peek(ss, STACK_TWO, &peek_val)) {
        printf("Peek at Stack 2 top: %d\n", peek_val);
    }
    printf("\n");

    // 4. 从栈1弹出一个元素
    int pop_val;
    printf("Popping from Stack 1...\n");
    if (shared_stack_pop(ss, STACK_ONE, &pop_val)) {
        printf("Popped value: %d\n", pop_val);
    }
    print_stack_status(ss);

    // 5. 将栈填满
    printf("Filling the rest of the stack from Stack 1...\n");
    int val = 40;
    while (!shared_stack_is_full(ss)) {
        printf("Pushing %d to Stack 1.\n", val);
        shared_stack_push(ss, STACK_ONE, &val);
        val += 10;
    }
    print_stack_status(ss);

    // 6. 尝试在已满的栈中继续压入
    printf("Attempting to push 999 to Stack 2 (should fail)...\n");
    int extra = 999;
    if (!shared_stack_push(ss, STACK_TWO, &extra)) {
        printf("Push failed as expected.\n\n");
    }

    // 7. 销毁栈
    printf("Destroying stack...\n");
    shared_stack_destroy(&ss);
    printf("Stack pointer is now: %s\n", ss == NULL ? "NULL" : "Not NULL");

    return 0;
}