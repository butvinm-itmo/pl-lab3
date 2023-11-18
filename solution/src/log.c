#include "log.h"

#include <stdarg.h>
#include <stdio.h>

LogLevel LOG_LEVEL = LOG_ERROR;

static const char *LEVEL_LABELS[] = {
    [LOG_INFO] = "[INFO] ",
    [LOG_ERROR] = "[ERROR] ",
    [LOG_DEBUG] = "[DEBUG] ",
};

void flogf(FILE *stream, LogLevel level, char const *message, ...) {
    if (level <= LOG_LEVEL) {
        va_list argptr;
        va_start(argptr, message);
        fputs(LEVEL_LABELS[level], stream);
        vfprintf(stream, message, argptr);
        fputs("\n", stream);
        va_end(argptr);
    }
}
