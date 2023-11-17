#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdlib.h>

#define pixel_at(img, x, y) ((img).pixels + (x) + (y) * (img).width)

/* Pixel with 1-byte color depth. */
typedef struct {
    uint8_t r, g, b;
} pixel;

/* RGB image. */
typedef struct {
    uint32_t width, height;
    pixel *pixels;
} image;

/* Return required size for image pixels data. */
size_t calc_image_size(uint32_t width, uint32_t height);

/* Create image structure with pre-allocated data.
 * If memory cannot be allocated, image pixels is NULL.
 */
image create_image(uint32_t width, uint32_t height);

#endif /* IMAGE_H */
