#include "processing/rotation.h"
#include "image.h"
#include "processing/copy.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

/* Parse rotation angle from string. */
AngleParseResult parse_rotation_angle(char *str) {
    char *endptr;
    errno = 0;
    long parsed_angle = strtol(str, &endptr, 10);
    // Validation from docs (man strtol).
    if (errno != 0 || endptr == str || *endptr != '\0') {
        return (AngleParseResult){false};
    }
    switch (parsed_angle) {
    case 270:
    case -90:
        return (AngleParseResult){true, ROT_ANGLE_270};
    case 180:
    case -180:
        return (AngleParseResult){true, ROT_ANGLE_180};
    case 90:
    case -270:
        return (AngleParseResult){true, ROT_ANGLE_90};
    case 0:
        return (AngleParseResult){true, ROT_ANGLE_0};
    default:
        return (AngleParseResult){false};
    }
}

Image _rotate_90(const Image img) {
    Image new_image = create_image(img.height, img.width);
    // clang-format off
    foreach_pixel(
        img,
        *pixel_at(new_image, j, i) = *pixel_at(img, img.width - 1 - i, j);
    );
    // clang-format on
    return new_image;
}

Image _rotate_180(const Image img) {
    Image new_image = create_image(img.width, img.height);
    // clang-format off
        foreach_pixel(
            img,
            *pixel_at(new_image, i, j) = *pixel_at(
                img,
                (img.width - 1 - i),
                img.height - 1 - j
            );
        );
    // clang-format on
    return new_image;
}

Image _rotate_270(const Image img) {
    Image new_image = create_image(img.height, img.width);
    // clang-format off
    foreach_pixel(
        img,
        *pixel_at(new_image, j, i) = *pixel_at(img, i, img.height - 1 - j);
    );
    // clang-format on
    return new_image;
}

Image rotate_image(const Image img, RotationAngle angle) {
    /* Rotate image data. */
    switch (angle) {
    case ROT_ANGLE_0:
        return copy_image(img);
    case ROT_ANGLE_90:
        return _rotate_90(img);
    case ROT_ANGLE_180:
        return _rotate_180(img);
    case ROT_ANGLE_270:
        return _rotate_270(img);
    }
}
