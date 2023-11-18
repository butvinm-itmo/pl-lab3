#include "image.h"

#include <stdint.h>
#include <stdlib.h>

size_t calc_image_size(uint32_t width, uint32_t height) {
    return height * width * sizeof(Pixel);
}

Image create_image(uint32_t width, uint32_t height) {
    return (Image){
        .width = width,
        .height = height,
        .pixels = malloc(calc_image_size(height, width)),
    };
}

void destroy_image(Image img) {
    free(img.pixels);
}
