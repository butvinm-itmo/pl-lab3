#ifndef BMP_H
#define BMP_H

#include "image.h"
#include "result.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    /* Image read properly. */
    FROM_BMP_OK = 0,
    /* Unsupported or broken header. */
    FROM_BMP_INVALID_HEADER,
    /* Not BMP format. */
    FROM_BMP_UNSUPPORTED_FORMAT,
    /* Unsupported compression. */
    FROM_BMP_UNSUPPORTED_COMPRESSION,
    /* Unsupported color depth. */
    FROM_BMP_UNSUPPORTED_COLOR_DEPTH,
    /* Cannot read pixel data. */
    FROM_BMP_INVALID_PIXELS,
    /* Not enough memory for image */
    FROM_BMP_CANNOT_ALLOC_MEMORY,
} FromBmpStatus;

Result(FromBmpResult, Image, FromBmpStatus);

/* Read BMP image from file. */
FromBmpResult from_bmp(FILE *in);

typedef enum {
    /* Image read properly. */
    TO_BMP_OK = 0,
    /* Writing tio file failed. */
    TO_BMP_FAILED,
} ToBmpStatus;

/* Write image to file in BMP format. */
ToBmpStatus to_bmp(FILE *out, Image img);

#endif /* BMP_H */
