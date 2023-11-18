#include "image.h"

#include "result.h"

#include <stdint.h>
#include <stdlib.h>

MaybeImpl(Image);

size_t calc_image_size(uint32_t width, uint32_t height) {
    return height * width * sizeof(Pixel);
}

MaybeImage create_image(uint32_t width, uint32_t height) {
    Pixel *pixels = malloc(calc_image_size(height, width));
    if (pixels == NULL) {
        return NoneImage;
    }
    const Image img = {
        .width = width,
        .height = height,
        .pixels = pixels,
    };
    return SomeImage(img);
}

void destroy_image(Image img) {
    if (img.pixels != NULL) {
        free(img.pixels);
        img.pixels = NULL;
    }
}
