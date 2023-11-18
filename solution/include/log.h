#ifndef LOG_H
#define LOG_H

#include "string.h"
#include <stdarg.h>
#include <stdio.h>

/* Log level. */
typedef enum { LOG_ERROR = 0, LOG_INFO, LOG_DEBUG } LogLevel;

/* Set to log everything upon that level. */
extern LogLevel LOG_LEVEL;

/* General logger. */
void flogf(FILE *stream, LogLevel level, char const *message, ...);

#define INFO(message) flogf(stdout, LOG_INFO, message)
#define INFOF(message, ...) flogf(stdout, LOG_INFO, message, __VA_ARGS__)

#define DEBUG(message) flogf(stdout, LOG_DEBUG, message)
#define DEBUGF(message, ...) flogf(stdout, LOG_DEBUG, message, __VA_ARGS__)

#define ERROR(message) flogf(stderr, LOG_ERROR, message)
#define ERRORF(message, ...) flogf(stderr, LOG_ERROR, message, __VA_ARGS__)

#endif
