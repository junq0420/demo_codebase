#include <stdlib.h>
#include "./inc/app.h"

int main(void) {
	if (!app_init()) {
		return EXIT_FAILURE;
	}

	if (!app_execute()) {
		app_exit();
		return EXIT_FAILURE;
	}

	app_exit();

	return EXIT_SUCCESS;
}
