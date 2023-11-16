#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

/* Pixel with 1-byte color depth. */
typedef struct {
    uint8_t r, g, b;
} pixel;

/* RGB image. */
typedef struct {
    uint32_t width, height;
    pixel *pixels;
} image;

#endif /* IMAGE_H */
