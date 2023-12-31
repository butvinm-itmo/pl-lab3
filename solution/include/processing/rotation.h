#ifndef ROTATION_H
#define ROTATION_H

#include "image.h"
#include "result.h"

/* Possible image rotation angles. */
typedef enum {
    ROT_ANGLE_0 = 0,
    ROT_ANGLE_90 = 90,
    ROT_ANGLE_180 = 180,
    ROT_ANGLE_270 = 270,
} RotationAngle;

Maybe(RotationAngle);

/* Parse rotation angle from string. */
MaybeRotationAngle parse_rotation_angle(char const *str);

/* Rotate image. */
MaybeImage rotate_image(Image img, RotationAngle angle);

#endif
