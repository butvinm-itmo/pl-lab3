#ifndef ROTATION_H
#define ROTATION_H

#include "image.h"
#include "result.h"

#include <stdbool.h>

/* Possible image rotation angles. */
typedef enum {
    ROT_ANGLE_0 = 0,
    ROT_ANGLE_90 = 90,
    ROT_ANGLE_180 = 180,
    ROT_ANGLE_270 = 270,
} RotationAngle;

Result(AngleParseResult, RotationAngle, bool);

/* Parse rotation angle from string. */
AngleParseResult parse_rotation_angle(char *str);

/* Rotate image date inplace. */
Image rotate_image(const Image img, RotationAngle angle);

#endif
