#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "../inc/logger.h"

static FILE *logFile = NULL;
static LogLevel currentLevel = LOG_INFO;


bool logger_init(const char *logFilePath) {
	if (logFilePath == NULL) {
		logFile = stdout;
	} else {
		logFile = fopen(logFilePath, "a");
		if (logFile == NULL || errno) {
			fprintf(stderr, "Failed to open %s file, and error msg is %s\n", logFilePath, strerror(errno));
			return false;
		}
	}

	return true;
}


void logger_close(void) {
	if (logFile && logFile != stdout) {
		fclose(logFile);
	}
	logFile = NULL;
}


void logger_msg(LogLevel level, const char *format, ...) {
	// if the log level is lower than the current log system level, or the log file is not open, then no info will be recorded
	if (level < currentLevel || !logFile) {
		return;
	}

	// write formatted time string
	char time_buf[20];
	time_t now = time(NULL);
	struct tm *now_tm = localtime(&now);
	strftime(time_buf, 20, "%Y-%m-%d %H:%M:%S", now_tm);
	fprintf(logFile, "[%s] ", time_buf);

	// write log level
	switch (level) {
		case LOG_DEBUG: 	fprintf(logFile, "[DEBUG] "); 	break;
		case LOG_INFO: 		fprintf(logFile, "[INFO] "); 	break;
		case LOG_WARNING: 	fprintf(logFile, "[WARNING] "); break;
		case LOG_ERROR: 	fprintf(logFile, "[ERROR] "); 	break;
		case LOG_FATAL: 	fprintf(logFile, "[FATAL] "); 	break;
		default: 			fprintf(logFile, "[UNKNOWN] "); break;
	}

	va_list args;
	va_start(args, format);
	vfprintf(logFile, format, args);
	va_end(args);

	fprintf(logFile, "\n");
	fflush(logFile); // flush the buffer to ensure that the log info is written to file
}


void logger_set_level(LogLevel level) {
	currentLevel = level;
}

