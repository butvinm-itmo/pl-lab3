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
    /* Cannot write to image structure. */
    FROM_BMP_BAD_IMAGE_PTR,
    /* Not enough memory for image */
    FROM_BMP_CANNOT_ALLOC_MEMORY,
} from_bmp_result;

/* Read BMP image from file. */
from_bmp_result from_bmp(FILE *in, image *img);

typedef enum class {
    /* Image read properly. */
    TO_BMP_OK = 0,
    /* Writing tio file failed. */
    TO_BMP_FAILED,
} to_bmp_result;

/* Write image to file in BMP format. */
to_bmp_result to_bmp(FILE *out, image *img);

#endif /* BMP_H */
