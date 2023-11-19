#ifndef EXIT_CODES_H
#define EXIT_CODES_H

#include <stdlib.h>

/* Application exit codes. */
typedef enum {
    /* Program done successfully. */
    EXIT_OK = EXIT_SUCCESS,
    /* Error caused by wrong usage. */
    EXIT_WRONG_USAGE,
    /* Error during image reading. */
    EXIT_READ,
    /* Error during processing. */
    EXIT_PROC,
    /* Error during image saving. */
    EXIT_WRITE,
} ExitCodes;

#endif
