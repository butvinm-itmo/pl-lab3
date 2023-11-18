#include "processing/copy.h"
#include "image.h"

Image copy_image(const Image img) {
    Image new_img = create_image(img.width, img.height);
    foreach_pixel(img, *pixel_at(new_img, i, j) = *pixel_at(img, i, j););
    return new_img;
}
