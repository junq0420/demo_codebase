#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdarg.h>
#include <stdbool.h>
#include "types.h"

// Log Level
typedef enum {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL
} LogLevel;

/**
  * init logger module
  * @param logFilePath specify the log file path. if it is NULL, the default out is to STDOUT
  * @retval returns true if successful, false if failed
 **/
bool logger_init(const char *logFilePath);


/**
  * close logger module
 **/
void logger_close(void);


/**
  * write log with logger module
  * @param level the log level
  * @param format format string for log message
  * @param ... the variadic list
 **/
void logger_msg(LogLevel level, const char *format, ...);


/**
  * set the current log level
  * @param level the new level
 **/
void logger_set_level(LogLevel level);

#endif // __LOGGER_H__ 
