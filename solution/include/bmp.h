#ifndef BMP_H
#define BMP_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "image.h"

typedef enum {
    /* Image read properly. */
    OK = 0,
    /* Unsupported or broken header. */
    INVALID_HEADER,
    /* Not BMP format. */
    UNSUPPORTED_FORMAT,
    /* Unsupported compression. */
    UNSUPPORTED_COMPRESSION,
    /* Unsupported color depth. */
    UNSUPPORTED_COLOR_DEPTH,
    /* Cannot read pixel data. */
    INVALID_PIXELS,
    /* Cannot write to image structure. */
    BAD_IMAGE_PTR,
} read_result;

/* Read BMP image from file. */
read_result from_bmp(FILE *in, image *img);

#endif /* BMP_H */
