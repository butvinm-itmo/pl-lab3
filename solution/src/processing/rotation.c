#include "processing/rotation.h"

#include "image.h"
#include "processing/copy.h"

#include <errno.h>
#include <stdio.h>

MaybeImpl(RotationAngle);

/* Parse rotation angle from string. */
MaybeRotationAngle parse_rotation_angle(char const *str) {
    char *endptr;
    errno = 0;
    const long angle = strtol(str, &endptr, 10);

    // Validation from docs (man strtol).
    if (errno != 0 || endptr == str || *endptr != '\0') {
        return NoneRotationAngle;
    }

    switch (angle) {
    case 270:
    case -90:
        return SomeRotationAngle(ROT_ANGLE_270);
    case 180:
    case -180:
        return SomeRotationAngle(ROT_ANGLE_180);
    case 90:
    case -270:
        return SomeRotationAngle(ROT_ANGLE_90);
    case 0:
        return SomeRotationAngle(ROT_ANGLE_0);
    default:
        return NoneRotationAngle;
    }
}

static MaybeImage _create_rotated_image(Image img, RotationAngle angle) {
    switch (angle) {
    case ROT_ANGLE_0:
        return copy_image(img);
    case ROT_ANGLE_90:
        return create_image(img.height, img.width);
    case ROT_ANGLE_180:
        return create_image(img.width, img.height);
    case ROT_ANGLE_270:
        return create_image(img.height, img.width);
    }
}

/* Rotate image data. */
MaybeImage rotate_image(Image img, RotationAngle angle) {
    MaybeImage new_image = _create_rotated_image(img, angle);
    if (!new_image.status || img.pixels == NULL) {
        return NoneImage;
    }
    // clang-format off
    switch (angle) {
    case ROT_ANGLE_0:
        break;
    case ROT_ANGLE_90:
        foreach_pixel(
            img,
            *pixel_at(new_image._, j, i) = *pixel_at(img, img.width - 1 - i, j);
        );
        break;
        break;
    case ROT_ANGLE_180:
        foreach_pixel(
            img,
            *pixel_at(new_image._, i, j) = *pixel_at(img, (img.width - 1 - i), img.height - 1 - j);
        );
        break;
    case ROT_ANGLE_270:
        foreach_pixel(
            img,
            *pixel_at(new_image._, j, i) = *pixel_at(img, i, img.height - 1 - j);
        );
    }
    // clang-format on
    return new_image;
}
