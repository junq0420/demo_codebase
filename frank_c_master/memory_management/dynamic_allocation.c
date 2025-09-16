#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char *name;
	int level;
	int hp;
	double skill_cd;
} Character;

typedef struct {
	char *name;
	int *task_list;
	int task_count;
} Employee;

/* Compare the difference of usage between statical array with dynamical array */
void static_dynamic_diff(void);

/* Create game character to show malloc() usage */
Character *create_character(const char *name, int level, int hp, double skill_cd);
void free_character(Character *character);

/* Reallocate department_budgets to show realloc() usage */
void department_budgets(void);
void print_budgets(int *budgets, size_t size);

/* Create employee task list with initial value to show calloc() usage */
Employee *create_employee(const char *name, int task_count);
void free_employee(Employee *employee);
void print_employee(Employee *employee);

int main(void)
{
    static_dynamic_diff();
	puts("");

	Character *hero = create_character("Hero", 1, 100, 5.8);
	if (hero == NULL) { return EXIT_FAILURE; }

	printf("Character Name: %s\nCharacter Level: %d\nCharacter HP: %d\nCharacter Skill Cold: %.1f\n", 
		hero->name, hero->level, hero->hp, hero->skill_cd);

	free_character(hero);
	puts("");

	department_budgets();
	puts("");

	Employee *employee = create_employee("Tom", 5);
	if (employee == NULL) { return EXIT_FAILURE; }

	employee->task_list[0] = 2;
	employee->task_list[1] = 1;
	employee->task_list[2] = 3;
	print_employee(employee);

    return EXIT_SUCCESS;
}


void static_dynamic_diff(void) {
    // statically create array
    int static_array[5] = {1, 2, 3, 4, 5};

	puts("static array: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", static_array[i]);
    }
	puts("\n");

	// dynamically create array
	int *dynamic_array = (int *)malloc(5 * sizeof(int));

	for (size_t i = 0; i < 5; i++) {
		dynamic_array[i] = i + 1;
	}
	puts("dynamical array: ");
	for (size_t i = 0; i < 5; i++) {
		printf("%d ", dynamic_array[i]);
	}
}

void department_budgets(void) {
	int *budgets = (int *)malloc(3 * sizeof(int));
	if (budgets == NULL) {
		perror("Failed to malloc for budgets array");
		exit(EXIT_FAILURE);
	}

	budgets[0] = 14000;
	budgets[1] = 15000;
	budgets[2] = 12000;

	puts("every department budgets summary in the first year");
	print_budgets(budgets, 3);

	// add 2 departments in the second year
	// re-allocate memory for budgets by used realloc()
	int *new_budgets = (int *)realloc(budgets, 5);
	if (new_budgets == NULL) {
		perror("Failed to realloc for new_budgets");
		free(budgets); // free original budgets pointer if new_budgets realloc failed
		exit(EXIT_FAILURE);
	}

	budgets = new_budgets;
	budgets[3] = 18000;
	budgets[4] = 17000;

	puts("every department budgets summary in the second year");
	print_budgets(budgets, 5);

	free(budgets);
}

void print_budgets(int *budgets, size_t size) {
	for (size_t i = 0; i < size; i++) {
		printf("department[%zd]: %d\n", i, budgets[i]);
	}
}

Character *create_character(const char *name, int level, int hp, double skill_cd) {
	Character *character = (Character *)malloc(sizeof(Character));
	if (character == NULL) {
		perror("Failed to malloc for character object");
		return NULL;
	}

	character->name = (char *)malloc(strlen(name) + 1);
	if (character->name == NULL) {
		perror("Failed to malloc for character name");
		free(character); // free character memory before the return of function
		return NULL;
	}
	strncpy(character->name, name, strlen(name) + 1);

	character->level = level;
	character->hp = hp;
	character->skill_cd = skill_cd;

	return character;
}

void free_character(Character *character) {
	if (character != NULL) {
		free(character->name);
		free(character);
	}
}


Employee *create_employee(const char *name, int task_count) {
	Employee *new_employee = (Employee *)malloc(sizeof(Employee));
	if (new_employee == NULL) {
		perror("Failed to malloc employee object");
		return NULL;
	}

	new_employee->name = (char *)malloc(strlen(name) + 1);
	if (new_employee->name == NULL) {
		perror("Failed to malloc for employee name");
		free(new_employee);
		return NULL;
	}
	strncpy(new_employee->name, name, strlen(name) + 1);

	new_employee->task_list = (int *)calloc(task_count, sizeof(int));
	if (new_employee->task_list == NULL) {
		perror("Failed to calloc for employee task list");
		free(new_employee->name);
		free(new_employee); // free all allocated memory if current allocation is failure
		return NULL;
	}

	new_employee->task_count = task_count;


	return new_employee;
}

void free_employee(Employee *employee) {
	if (employee != NULL) {
		free(employee->task_list);
		free(employee->name);
		free(employee);
	}
}

void print_employee(Employee *employee) {
	printf("Employee %s has %d task count\n", employee->name, employee->task_count);
	for (size_t i = 0; i < employee->task_count; i++) {
		printf("task_list[%zd]: %d\n", i, employee->task_list[i]);
	}
}


