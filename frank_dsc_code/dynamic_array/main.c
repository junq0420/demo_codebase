#include <stdio.h>
#include <string.h>
#include "dynamic_array.h"

void print_data(const Data *data)
{
	printf("{age = %d, name = %s}, ", data->age, data->name);
}

int main(void)
{
	Data new_data = { .age = 20, .name = "Alice" };

	DynamicArray *my_arr = create_array(2);
	array_append(my_arr, new_data);
	print_array(my_arr, print_data);

	new_data.age = 18;
	memcpy(new_data.name, "Bob", sizeof("Bob"));
	array_append(my_arr, new_data);
	print_array(my_arr, print_data);

	new_data.age = 22;
	memcpy(new_data.name, "Charles", sizeof("Charles"));
	array_append(my_arr, new_data);
	print_array(my_arr, print_data);

	printf("Insert at beginning.......\n");
	new_data.age = 21;
	memcpy(new_data.name, "Dave", sizeof("Dave"));
	array_insert(my_arr, 0, new_data);
	print_array(my_arr, print_data);

	printf("Delete.......\n");
	array_delete(my_arr, 0);
	print_array(my_arr, print_data);

	printf("Insert at endding.......\n");
	new_data.age = 23;
	memcpy(new_data.name, "Ele", sizeof("Ele"));
	array_insert(my_arr, 3, new_data);
	print_array(my_arr, print_data);

	printf("Update at beginning.......\n");
	new_data.age = 30;
	memcpy(new_data.name, "Frank", sizeof("Frank"));
	array_update(my_arr, 0, new_data);
	print_array(my_arr, print_data);

	printf("Update at endding.......\n");
	new_data.age = 25;
	memcpy(new_data.name, "Golf", sizeof("Golf"));
	array_update(my_arr, 3, new_data);
	print_array(my_arr, print_data);
	//
	//
	array_delete(my_arr, 0);
	array_delete(my_arr, 0);
	array_delete(my_arr, 0);
	print_array(my_arr, print_data);

	destroy_array(my_arr);

	return 0;
}
