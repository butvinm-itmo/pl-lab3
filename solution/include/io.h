#ifndef IO_H
#define IO_H

#include "result.h"

#define IO_RESULT(name, value_t) RESULT(name, value_t, io_state)

/* IO operations statuses. */
typedef enum {
    /* Keep quite, everything is fine. */
    IO_OK = 0,
    /* Stream opening error. */
    IO_OPEN_ERR,
    /* Stream closing error. You may lost written data probably. */
    IO_CLOSE_ERR,
} io_state;

#endif
