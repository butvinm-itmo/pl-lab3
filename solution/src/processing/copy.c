#include "processing/copy.h"

#include "image.h"

Image copy_image(Image img) {
    Image new_img = create_image(img.width, img.height);
    if (new_img.pixels != NULL) {
        foreach_pixel(img, *pixel_at(new_img, i, j) = *pixel_at(img, i, j););
    }
    return new_img;
}
