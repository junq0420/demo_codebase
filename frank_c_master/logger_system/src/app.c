#include "../inc/app.h"
#include "../inc/logger.h"
#include "../inc/error_handle.h"
#include "../inc/pointer_safety.h"
#include "../inc/memory_manager.h"


bool app_init(void) {
	// initialize logger system
	if (!logger_init("./app.log")) {
		perror("Failed to initialize logger.");
		return false;
	}

	logger_set_level(LOG_DEBUG);
	logger_msg(LOG_INFO, "Application initializing... ");

	return true;
}


bool app_execute(void) {
	logger_msg(LOG_INFO, "start to exexute application.");

	int *data = safe_malloc(sizeof(int));
	if (is_nullptr(data)) {
		log_error("Faied to safe_malloc for data.");
		return false;
	}

	logger_msg(LOG_INFO, "successfully allocate memory for data, now setting value.");
	*data = 69;

	printf("data is %d\n", SAFE_DEREF(data, 0));

	logger_msg(LOG_INFO, "successfully set value for data.");

	safe_free((void **)&data);
	logger_msg(LOG_DEBUG, "Finish application execution.");
	return true;
}


void app_exit(void) {
	logger_msg(LOG_DEBUG, "application ready to exit.");
	logger_close();
}
