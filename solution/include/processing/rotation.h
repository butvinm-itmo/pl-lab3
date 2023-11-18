#ifndef ROTATION_H
#define ROTATION_H

#include "image.h"
#include "result.h"

#include <stdbool.h>

/* Possible image rotation angles. */
typedef enum {
    ANGLE_0 = 0,
    ANGLE_90 = 90,
    ANGLE_180 = 180,
    ANGLE_270 = 270,
} rot_angle;

RESULT(rot_angle, bool)
static const rot_angle_result rot_angle_err = {.status = false};

/* Parse rotation angle from string. */
rot_angle_result parse_rot_angle(char *str);

/* Rotate image date inplace. */
image rotate_image(const image img, rot_angle angle);

#endif