#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

typedef enum {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
} LogLevel;

const char *get_current_time();
void log_msg(LogLevel level, const char *format, ...);

int main(void) {
	log_msg(LOG_INFO, "This is an info msg");
	log_msg(LOG_WARNING, "This is a warning msg with invaild syntax");
	log_msg(LOG_ERROR, "This is an error msg with segment fault");
	log_msg(LOG_INFO, "This is another info msg");

	return EXIT_SUCCESS;
}

const char *get_current_time() {
	static char time_buffer[20];

	time_t now = time(NULL);
	struct tm *tm_info = localtime(&now);

	strftime(time_buffer, 20, "%Y-%m-%d %H:%M:%S", tm_info);

	return time_buffer;
}

void log_msg(LogLevel level, const char *format, ...) {
	char *log_str;

	switch (level) {
		case LOG_INFO: fwrite("\x1b[32m", sizeof(char), 5, stdout); log_str = "INFO"; break;
		case LOG_WARNING: fwrite("\x1b[33m", sizeof(char), 5, stdout);log_str = "WARNING"; break;
		case LOG_ERROR: fwrite("\x1b[31m", sizeof(char), 5, stdout);log_str = "ERROR"; break;
		default: log_str = "UNKNOWN"; break;
	}

	printf("[%s] [%s]: ", get_current_time(), log_str);
	fwrite("\x1b[39m", sizeof(char), 5, stdout); // set the display color the terminal console

	va_list args;
	va_start(args, format);
	vprintf(format, args);

	va_end(args);

	printf("\n");
}
