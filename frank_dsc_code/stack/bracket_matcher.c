// 应用链式栈（容量不受限）实现括号匹配器
#include <stdio.h>
#include <string.h>

#include "linked_stack.h"


/**
 * 判断字符是否为开括号
 * 
 * @param c 待判断的字符
 * @return bool 如果字符是'('、'['或'{'中的任意一个则返回true，否则返回false
 */
bool is_opener(char c)
{
    return c == '(' || c == '[' || c == '{';
}

bool matches(char opener, char closer)
{
    return (opener == '(' && closer == ')') ||
           (opener == '[' && closer == ']') ||
           (opener == '{' && closer == '}');
}

bool check_brackets(const char *code)
{
    Stack *stack = stack_create(20, sizeof(char), NULL);
    if (stack == NULL) {
        fprintf(stderr, "Failed to create stack for bracket checking.\n");
        return false;
    }

    size_t len = strlen(code);
    for (size_t i = 0; i < len; i++) {
        char current_char = code[i];

        if (is_opener(current_char)) {// 若为开括号，则压入栈
            stack_push(stack, &current_char);
        } else if (current_char == ')' || current_char == ']' || current_char == '}') {// 闭括号检查栈顶元素
            // 1. 错误情况：栈为空，闭括号多余
            if (stack_is_empty(stack)) {
                fprintf(stderr, "Unmatched closing bracket '%c' found.\n", current_char);
                stack_destroy(&stack);
                return false;
            }

            char poped_opener;
            stack_pop(stack, &poped_opener);// 2. 栈非空，则弹出栈顶元素

            // 3. 错误情况：栈非空，开括号与闭括号不匹配
            if (!matches(poped_opener, current_char)) {
                fprintf(stderr, "Mismatched brackets: '%c' does not match '%c'", poped_opener, current_char);
                stack_destroy(&stack);
                return false;
            }
        }
    }

    // 4. 错误情况：栈非空，多余开括号未关闭
    if (!stack_is_empty(stack)) {
        char unclosed_opener;
        stack_peek(stack, &unclosed_opener);
        fprintf(stderr, "Unmatched opening bracket '%c' remain in stack.\n", unclosed_opener);
        stack_destroy(&stack);
        return false;
    }

    stack_destroy(&stack);
    return true;
}

void check() {
	const char* test_cases[] = {
	   "int main() { int x = (1 + 2); return 0; }", // 有效
	   "void func(int a[]);",                        // 有效
	   "([{}])",                                     // 有效
	   "int arr[5] = {1, 2, 3};",                     // 有效
	   "",                                           // 有效 (空字符串)
	   "abc",                                        // 有效 (无括号)
	   "([)]",                                       // 无效: 交叉不匹配
	   "((()",                                       // 无效: 开括号未关闭
	   "())",                                        // 无效: 闭括号无对应
	   "if (x > 0) { printf(\"hello\");",             // 无效: } 未关闭
	   "int y = { ( [ ] ) };"                         // 有效: 复杂嵌套
	};

	int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

	for (int i = 0; i < num_cases; i++) {
		printf("正在检查: \"%s\"\n", test_cases[i]);
		if (check_brackets(test_cases[i])) {
			printf("结果: -> 有效\n\n");
		}
		else {
			printf("结果: -> 无效\n\n");
		}
	}
}

int main(void)
{
    check();
    return 0;
}