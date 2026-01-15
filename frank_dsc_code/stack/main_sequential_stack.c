#include <stdio.h>
#include <string.h>
#include "sequential_stack.h"

// --- 打印函数 ---
// 用于打印整数
void print_int(const void *data)
{
	if (data == NULL) {
		printf("NULL");
		return;
	}
	printf("%d", *(int *)data);
}

// 用于打印字符
void print_char(const void *data)
{
	if (data == NULL) {
		printf("NULL");
		return;
	}
	printf("%c", *(char *)data);
}

// 用于打印字符串
void print_string(const void *data)
{
	if (data == NULL) {
		printf("NULL");
		return;
	}
	printf("%s", (const char *)data);
}

// --- 辅助函数 ---
void print_stack_state(Stack *stack, const char *title)
{
	printf("\n=== %s ===\n", title);
	if (stack == NULL) {
		printf("Stack is NULL\n");
		return;
	}
	printf("Size: %zu, Capacity: %zu, Is Empty: %s, Is Full: %s\n",
		   stack_get_size(stack),
		   stack_get_capacity(stack),
		   stack_is_empty(stack) ? "Yes" : "No",
		   stack_is_full(stack) ? "Yes" : "No");
}

// --- 测试用例 1: 测试整数栈 ---
void test_integer_stack(void)
{
	printf("\n\n========== TEST 1: INTEGER STACK ==========\n");

	// 1. 测试 stack_create
	printf("\n--- Test stack_create ---\n");
	Stack *int_stack = stack_create(5, sizeof(int), print_int);
	if (int_stack == NULL) {
		printf("Failed to create integer stack\n");
		return;
	}
	printf("Successfully created integer stack with capacity 5\n");
	print_stack_state(int_stack, "After creation");

	// 2. 测试 stack_is_empty
	printf("\n--- Test stack_is_empty ---\n");
	printf("Is stack empty? %s\n", stack_is_empty(int_stack) ? "Yes" : "No");

	// 3. 测试 stack_push
	printf("\n--- Test stack_push ---\n");
	int values[] = {10, 20, 30, 40, 50};
	for (int i = 0; i < 5; i++) {
		if (stack_push(int_stack, &values[i])) {
			printf("Pushed %d successfully\n", values[i]);
		} else {
			printf("Failed to push %d\n", values[i]);
		}
	}
	print_stack_state(int_stack, "After pushing 5 elements");

	// 4. 测试 stack_is_full
	printf("\n--- Test stack_is_full ---\n");
	printf("Is stack full? %s\n", stack_is_full(int_stack) ? "Yes" : "No");

	// 5. 测试 stack_peek
	printf("\n--- Test stack_peek ---\n");
	int peek_value = 0;
	if (stack_peek(int_stack, &peek_value)) {
		printf("Peeked value: %d\n", peek_value);
		printf("Stack size after peek: %zu (unchanged)\n", stack_get_size(int_stack));
	}

	// 6. 测试 stack_pop
	printf("\n--- Test stack_pop ---\n");
	int pop_value = 0;
	for (int i = 0; i < 3; i++) {
		if (stack_pop(int_stack, &pop_value)) {
			printf("Popped value: %d\n", pop_value);
		} else {
			printf("Failed to pop\n");
		}
	}
	print_stack_state(int_stack, "After popping 3 elements");

	// 7. 测试 stack_push 后继续操作
	printf("\n--- Test stack_push after pop ---\n");
	int new_value = 100;
	if (stack_push(int_stack, &new_value)) {
		printf("Pushed %d successfully\n", new_value);
	}
	print_stack_state(int_stack, "After pushing one more element");

	// 8. 测试 stack_destroy
	printf("\n--- Test stack_destroy ---\n");
	stack_destroy(&int_stack);
	if (int_stack == NULL) {
		printf("Stack successfully destroyed (pointer set to NULL)\n");
	}
}

// --- 测试用例 2: 测试字符栈 ---
void test_character_stack(void)
{
	printf("\n\n========== TEST 2: CHARACTER STACK ==========\n");

	// 1. 创建字符栈
	printf("\n--- Test stack_create for char ---\n");
	Stack *char_stack = stack_create(10, sizeof(char), print_char);
	if (char_stack == NULL) {
		printf("Failed to create character stack\n");
		return;
	}
	printf("Successfully created character stack with capacity 10\n");
	print_stack_state(char_stack, "After creation");

	// 2. 推入字符
	printf("\n--- Test stack_push with characters ---\n");
	char chars[] = {'A', 'B', 'C', 'D', 'E'};
	for (int i = 0; i < 5; i++) {
		if (stack_push(char_stack, &chars[i])) {
			printf("Pushed '%c' successfully\n", chars[i]);
		}
	}
	print_stack_state(char_stack, "After pushing 5 characters");

	// 3. Peek 和 Pop 操作
	printf("\n--- Test stack_peek and stack_pop ---\n");
	char peek_char = '\0';
	if (stack_peek(char_stack, &peek_char)) {
		printf("Peeked character: '%c'\n", peek_char);
	}

	char pop_char = '\0';
	while (!stack_is_empty(char_stack)) {
		if (stack_pop(char_stack, &pop_char)) {
			printf("Popped character: '%c'\n", pop_char);
		}
	}
	print_stack_state(char_stack, "After popping all elements");

	// 4. 清理
	stack_destroy(&char_stack);
	printf("\nCharacter stack destroyed\n");
}

// --- 测试用例 3: 测试字符串栈 ---
void test_string_stack(void)
{
	printf("\n\n========== TEST 3: STRING STACK ==========\n");

	// 1. 创建字符串栈（每个元素是50字节的字符数组）
	printf("\n--- Test stack_create for string ---\n");
	Stack *string_stack = stack_create(4, 50, print_string);
	if (string_stack == NULL) {
		printf("Failed to create string stack\n");
		return;
	}
	printf("Successfully created string stack with capacity 4\n");
	print_stack_state(string_stack, "After creation");

	// 2. 推入字符串
	printf("\n--- Test stack_push with strings ---\n");
	char str1[50] = "Hello";
	char str2[50] = "World";
	char str3[50] = "Stack";
	char str4[50] = "Test";

	if (stack_push(string_stack, str1)) printf("Pushed \"%s\"\n", str1);
	if (stack_push(string_stack, str2)) printf("Pushed \"%s\"\n", str2);
	if (stack_push(string_stack, str3)) printf("Pushed \"%s\"\n", str3);
	if (stack_push(string_stack, str4)) printf("Pushed \"%s\"\n", str4);

	print_stack_state(string_stack, "After pushing 4 strings (full)");

	// 3. 尝试超出容量的推入（应该失败）
	printf("\n--- Test stack_push on full stack ---\n");
	char str5[50] = "Extra";
	if (!stack_push(string_stack, str5)) {
		printf("Correctly prevented push on full stack\n");
	}

	// 4. Peek 和 Pop
	printf("\n--- Test stack_peek and stack_pop for strings ---\n");
	char peek_str[50] = {0};
	if (stack_peek(string_stack, peek_str)) {
		printf("Peeked string: \"%s\"\n", peek_str);
	}

	char pop_str[50] = {0};
	if (stack_pop(string_stack, pop_str)) {
		printf("Popped string: \"%s\"\n", pop_str);
	}

	print_stack_state(string_stack, "After popping one string");

	// 5. 清理
	stack_destroy(&string_stack);
	printf("\nString stack destroyed\n");
}

// --- 测试用例 4: 边界情况测试 ---
void test_edge_cases(void)
{
	printf("\n\n========== TEST 4: EDGE CASES ==========\n");

	// 1. 测试小容量栈
	printf("\n--- Test stack with capacity 1 ---\n");
	Stack *small_stack = stack_create(1, sizeof(int), print_int);
	if (small_stack == NULL) {
		printf("Failed to create small stack\n");
		return;
	}

	int value = 42;
	printf("Is stack empty? %s\n", stack_is_empty(small_stack) ? "Yes" : "No");
	printf("Is stack full? %s\n", stack_is_full(small_stack) ? "Yes" : "No");

	if (stack_push(small_stack, &value)) {
		printf("Pushed value: %d\n", value);
	}
	printf("Is stack full? %s\n", stack_is_full(small_stack) ? "Yes" : "No");

	// 2. 测试在空栈上执行 pop 和 peek（应该失败）
	printf("\n--- Test pop and peek on empty stack ---\n");
	int output = 0;
	stack_pop(small_stack, &output);  // 这会弹出唯一的元素
	
	if (!stack_pop(small_stack, &output)) {
		printf("Correctly prevented pop on empty stack\n");
	}
	if (!stack_peek(small_stack, &output)) {
		printf("Correctly prevented peek on empty stack\n");
	}

	// 3. 清理
	stack_destroy(&small_stack);
	printf("\nSmall stack destroyed\n");
}

// --- 测试用例 5: 错误处理测试 ---
void test_error_handling(void)
{
	printf("\n\n========== TEST 5: ERROR HANDLING ==========\n");

	// 1. 测试 NULL 指针
	printf("\n--- Test with NULL pointers ---\n");
	
	printf("Calling stack_create with capacity=0: ");
	Stack *invalid_stack = stack_create(0, sizeof(int), print_int);
	if (invalid_stack == NULL) {
		printf("Correctly returned NULL\n");
	}

	printf("Calling stack_create with element_size=0: ");
	invalid_stack = stack_create(5, 0, print_int);
	if (invalid_stack == NULL) {
		printf("Correctly returned NULL\n");
	}

	printf("Calling stack_create with print_func=NULL: ");
	invalid_stack = stack_create(5, sizeof(int), NULL);
	if (invalid_stack == NULL) {
		printf("Correctly returned NULL\n");
	}

	// 2. 创建一个有效的栈用于进一步测试
	Stack *test_stack = stack_create(5, sizeof(int), print_int);

	printf("\nCalling stack_push with NULL stack: ");
	int dummy = 0;
	if (!stack_push(NULL, &dummy)) {
		printf("Correctly returned false\n");
	}

	printf("Calling stack_push with NULL element_data: ");
	if (!stack_push(test_stack, NULL)) {
		printf("Correctly returned false\n");
	}

	printf("Calling stack_pop with NULL output_buffer: ");
	if (!stack_pop(test_stack, NULL)) {
		printf("Correctly returned false\n");
	}

	printf("Calling stack_peek with NULL output_buffer: ");
	if (!stack_peek(test_stack, NULL)) {
		printf("Correctly returned false\n");
	}

	// 3. 清理
	stack_destroy(&test_stack);
	printf("\nTest stack destroyed\n");
}

int main(void)
{
	printf("========================================\n");
	printf("SEQUENTIAL STACK COMPREHENSIVE TEST SUITE\n");
	printf("========================================\n");

	test_integer_stack();
	test_character_stack();
	test_string_stack();
	test_edge_cases();
	test_error_handling();

	printf("\n\n========================================\n");
	printf("ALL TESTS COMPLETED\n");
	printf("========================================\n");

	return 0;
}
