#include <stdlib.h>
#include "../inc/pointer_safety.h"


bool is_nullptr(const void *ptr) {
	if (ptr == NULL) {
		log_error("Detected a NULL pointer");
		return true;
	}

	return false;
}
