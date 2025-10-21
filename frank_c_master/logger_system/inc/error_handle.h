#ifndef __ERROR_HANDLE_H__
#define __ERROR_HANDLE_H__

#include <stdarg.h>
#include "logger.h"
#include "types.h"

void log_warning(const char *format, ...);
void log_error(const char *format, ...);
void log_fatal(const char *format, ...);

#endif // __ERROR_HANDLE_H__
