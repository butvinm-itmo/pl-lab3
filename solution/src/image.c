#include "image.h"

#include <stdint.h>
#include <stdlib.h>

size_t calc_image_size(uint32_t width, uint32_t height) {
    return height * width * sizeof(pixel);
}

image create_image(uint32_t width, uint32_t height) {
    return (image){
        .width = width,
        .height = height,
        .pixels = malloc(calc_image_size(height, width)),
    };
}
