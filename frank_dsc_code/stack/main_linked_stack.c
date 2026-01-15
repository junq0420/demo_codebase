#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_stack.h"

// user defined data type
typedef struct {
    char *name;
    int age;
} Person;

static Person *create_person(char *name, int age)
{
    Person *person = (Person *)malloc(sizeof(Person));
    if (person == NULL) {
        fprintf(stderr, "create_person() error: malloc() failed\n");
        return NULL;
    }

    person->name = strdup(name);
    person->age = age;

    return person;
}
static void free_person(void *element_data)
{
    if (element_data == NULL) {
        fprintf(stderr, "free_person() error: element_data is NULL\n");
        return;
    }

    Person *person = (Person *)element_data;
    printf("prepare to free person name: %s, age: %d\n", person->name, person->age);
    free(person);
}

static void print_person(void *element_data)
{
    if (element_data == NULL) {
        fprintf(stderr, "print_person() error: element_data is NULL\n");
        return;
    }

    Person *person = (Person *)element_data;
    printf("name: %s, age: %d\n", person->name, person->age);
}

void test_linked_stack(void)
{ 
	printf("\n\n========== TEST 1: CREATE STACK ==========\n");
    Stack *stack = stack_create(10, sizeof(Person), free_person);
    if (stack == NULL) {
        fprintf(stderr, "test_linked_stack() error: stack_create() failed\n");
        return;
    }

    printf("created stack with memory pool capacity: %lu\n", (size_t)10);
    printf("now stack is empty(%s), size: %lu\n", stack_is_empty(stack) ? "YES" : "NO", stack_get_size(stack));

	printf("\n\n========== TEST 2: PUSH SOME PERSON INFO ==========\n");
    Person *person1 = create_person("Tom", 18);
    stack_push(stack, person1);
    printf("pushed person info: name: %s, age: %d\n", person1->name, person1->age);
    printf("now stack is empty(%s), size: %lu\n", stack_is_empty(stack) ? "YES" : "NO", stack_get_size(stack));

    Person *person2 = create_person("Jerry", 20);
    stack_push(stack, person2);
    printf("pushed person info: name: %s, age: %d\n", person2->name, person2->age);
    printf("now stack is empty(%s), size: %lu\n", stack_is_empty(stack) ? "YES" : "NO", stack_get_size(stack));

    Person *person3 = create_person("Frank", 24);
    stack_push(stack, person3);
    printf("pushed person info: name: %s, age: %d\n", person3->name, person3->age);
    printf("now stack is empty(%s), size: %lu\n", stack_is_empty(stack) ? "YES" : "NO", stack_get_size(stack));


	printf("\n\n========== TEST 3: PRINT CURRENT STACK ELEMENTS ==========\n");
    printf("current stack elements:\n");
    stack_foreach(stack, print_person);
    printf("finished printing stack elements\n");
    Person peek_person;
    if (stack_peek(stack, &peek_person)) {
        printf("the top element is: (name: %s, age: %d)\n", peek_person.name, peek_person.age);
    } else {
        fprintf(stderr, "stack_peek() error\n");
    }


	printf("\n\n========== TEST 4: POP ALL ELEMENTS ==========\n");
    Person pop_person;
    size_t stack_size = stack_get_size(stack);
    for (size_t i = 0; i < stack_size; i++) {
        if (stack_pop(stack, &pop_person)) {
            printf("popped element: (name: %s, age: %d)\n", pop_person.name, pop_person.age);
        } else {
            fprintf(stderr, "stack_pop() error\n");
        }
    }
    printf("current stack elements:\n");
    stack_foreach(stack, print_person);
    printf("finished printing stack elements");


	printf("\n\n========== TEST 5: DESTROY STACK ==========\n");
    stack_destroy(&stack);
    printf("stack is destroyed\n");

}
int main(void)
{ 
	printf("========================================\n");
	printf("LINKED STACK COMPREHENSIVE TEST SUITE\n");
	printf("========================================\n");

    test_linked_stack();

	printf("\n\n========================================\n");
	printf("ALL TESTS COMPLETED\n");
	printf("========================================\n");

    return 0;
}