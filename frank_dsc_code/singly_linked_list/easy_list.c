#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int val;
	struct Node *next;
} Node;

Node *create_node(int val)
{
	Node *new_node = (Node *)malloc(sizeof(Node));
	if (!new_node) {
		perror("new node malloc failed.\n");
		return NULL;
	}

	new_node->val = val;
	new_node->next = NULL;

	return new_node;
}

void append_node(Node **head, int val)
{
	Node *new_node = create_node(val);
	Node *current;

	if (*head == NULL) {// 若 list 为空
		*head = new_node;// head 为二级指针方便此时修改 head
		return;
	}

	current = *head;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = new_node;
}

void prepend_node(Node **head, int val)
{
	Node *new_node = create_node(val);

	new_node->next = *head;
	*head = new_node;
}

Node *find_node(Node *head, int val)
{
	Node *current = head;

	while (current) {
		if (current->val == val) {
			printf("Find node by %d val.\n", val);
			return current;
		}
		current = current->next;
	}

	return NULL;
}

void update_node(Node *head, int old_val, int new_val)
{
	Node *current = head;

	Node *toupdate_node = find_node(head, old_val);
	if (toupdate_node) {
		toupdate_node->val = new_val;
		printf("Updated node from %d to %d.\n", old_val, new_val);
		return;
	}

	printf("Not found to be update node.\n");
}

void delete_node(Node **head, int val)
{
	Node *current = *head;
	Node *prev;

	if (current != NULL && current->val == val) {// 删除头节点
		*head = current->next;
		free(current);
		return;
	}

	while (current) {// 删除中间或尾节点
		if (current->val == val) {
			prev->next = current->next;
			free(current);
			return;
		}

		prev = current;
		current = current->next;
	}
}

void print_list(Node *head)
{
	Node *current = head;

	printf("List: [ ");
	while (current) {
		printf("%d => ", current->val);
		current = current->next;
	}
	printf("NULL ]\n");
}

void free_list(Node **head)
{
	Node *current = *head;
	Node *next_node;

	while (current->next) {
		next_node = current->next;
		free(current);
		current = next_node;
	}
	free(current);

	*head = NULL;
}

int main(void)
{
	Node *list = create_node(10);
	append_node(&list, 20);
	append_node(&list, 30);
	print_list(list);

	prepend_node(&list, 5);
	print_list(list);

	update_node(list, 5, 0);
	print_list(list);

	delete_node(&list, 20);
	print_list(list);

	delete_node(&list, 10);
	print_list(list);

	free_list(&list);
	print_list(list);

	return 0;
}

