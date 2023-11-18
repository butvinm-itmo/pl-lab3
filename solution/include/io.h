#ifndef IO_H
#define IO_H

#include "result.h"

#define IO_RESULT(name, value_t) Result(name, value_t, IOState)

/* IO operations statuses. */
typedef enum {
    /* Keep calm, everything is fine. */
    IO_OK = 0,
    /* Stream opening error. */
    IO_OPEN_ERR,
    /* Stream closing error. You may lost written data probably. */
    IO_CLOSE_ERR,
} IOState;

#endif
