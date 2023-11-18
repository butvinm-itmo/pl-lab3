#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdlib.h>

#define pixel_at(img, x, y) ((img).pixels + (x) + (y) * (img).width)

#define foreach_pixel(img, expression)                                         \
    for (uint32_t i = 0; i < (img).width; ++i) {                               \
        for (uint32_t j = 0; j < (img).height; ++j) {                          \
            expression;                                                        \
        }                                                                      \
    }

/* Pixel with 1-byte color depth. */
typedef struct {
    uint8_t r, g, b;
} Pixel;

/* RGB image. */
typedef struct {
    uint32_t width, height;
    Pixel *pixels;
} Image;

/* Return required size for image pixels data. */
size_t calc_image_size(uint32_t width, uint32_t height);

/* Create image structure with pre-allocated data.
 * If memory cannot be allocated, image pixels is NULL.
 */
Image create_image(uint32_t width, uint32_t height);

#endif /* IMAGE_H */
