#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

double average(int args_num, ...);

int main(void)
{
	printf("ave [2, 3, 4, 5]: %f\n", average(4, 2, 3, 4, 5));

	return EXIT_SUCCESS;
}


/**
 * average multiple int value
 * @param args_num the numbers of the int argument
 * @param ... actually pass arguments
 * @retval the average of arguments
 */
double average(int args_num, ...) {
	double sum = 0.0;

	va_list args;// declare a va_list variable
	va_start(args, args_num);// initialize va_list

	for (int i = 0; i < args_num; i++) {
		sum += va_arg(args, int);
	}

	va_end(args);// clean up va_list

	return args_num > 0 ? sum / args_num : 0.0;
}
