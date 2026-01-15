#include <stdio.h>
#include <stdlib.h>

// dynamic macro function
#define da_append(xs, x)\
	do {\
		if (xs.count >= xs.capacity) {\
			if (xs.capacity == 0) xs.capacity = 4;\
			else xs.capacity *= 2;\
			xs.items = realloc(xs.items, xs.capacity * sizeof(*xs.items));\
		}\
		xs.items[xs.count++] = x;\
	} while(0)

typedef struct {
	int *items;
	size_t count;
	size_t capacity;
} Numbers;

typedef struct {
	float *items;
	size_t count;
	size_t capacity;
} Prince;

typedef struct {
	const char **items;
	size_t count;
	size_t capacity;
} Names;

int main(void)
{
	Names xs = {0};

	da_append(xs, "Alice");
	da_append(xs, "Bob");
	da_append(xs, "Charles");
	da_append(xs, "Dave");
	da_append(xs, "Ele");
	da_append(xs, "Frank");

	for (size_t i = 0; i < xs.count; i++) printf("%s\n", xs.items[i]);

	return 0;
}
