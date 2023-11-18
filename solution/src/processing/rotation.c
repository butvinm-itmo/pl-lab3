#include "image.h"
#include "processing/copy.h"
#include "processing/rotation.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

/* Parse rotation angle from string. */
rot_angle_result parse_rot_angle(char *str) {
    char *endptr;
    errno = 0;
    long parsed_angle = strtol(str, &endptr, 10);
    // Validation from docs (man strtol).
    if (errno != 0 || endptr == str || *endptr != '\0') {
        return rot_angle_err;
    }
    switch (parsed_angle) {
    case 270:
    case -90:
        return (rot_angle_result){true, ANGLE_270};
    case 180:
    case -180:
        return (rot_angle_result){true, ANGLE_180};
    case 90:
    case -270:
        return (rot_angle_result){true, ANGLE_90};
    case 0:
        return (rot_angle_result){true, ANGLE_0};
    default:
        return rot_angle_err;
    }
}

image rotate_90(const image img) {
    image new_image = create_image(img.height, img.width);
    // clang-format off
    foreach_pixel(
        img,
        *pixel_at(new_image, j, i) = *pixel_at(img, img.width - 1 - i, j);
    );
    return new_image;
}

image rotate_180(const image img) {
    image new_image = create_image(img.width, img.height);
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

image rotate_270(const image img) {
    image new_image = create_image(img.height, img.width);
    // clang-format off
    foreach_pixel(
        img,
        *pixel_at(new_image, j, i) = *pixel_at(img, i, img.height - 1 - j);
    );
    // clang-format on
    return new_image;
}

image rotate_image(const image img, rot_angle angle) {
    /* Rotate image data. */
    switch (angle) {
    case ANGLE_0:
        return copy_image(img);
    case ANGLE_90:
        return rotate_90(img);
    case ANGLE_180:
        return rotate_180(img);
    case ANGLE_270:
        return rotate_270(img);
    }
}
