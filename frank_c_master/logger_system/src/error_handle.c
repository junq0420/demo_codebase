#include <stdlib.h>
#include "../inc/error_handle.h"


void log_warning(const char *format, ...) {
	va_list args;
	va_start(args, format);
	logger_msg(LOG_WARNING, format, args);
	va_end(args);
}

void log_error(const char *format, ...) {
	va_list args;
	va_start(args, format);
	logger_msg(LOG_ERROR, format, args);
	va_end(args);
}

void log_fatal(const char *format, ...) {
	va_list args;
	va_start(args, format);
	logger_msg(LOG_FATAL, format, args);
	va_end(args);

	exit(EXIT_FAILURE);
}
