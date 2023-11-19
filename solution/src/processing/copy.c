#include "processing/copy.h"

#include "image.h"

MaybeImage copy_image(Image img) {
    MaybeImage new_img = create_image(img.width, img.height);
    if (!new_img.status || img.pixels == NULL) {
        return NoneImage;
    }
    foreach_pixel(img, *pixel_at(new_img._, i, j) = *pixel_at(img, i, j););
    return new_img;
}
