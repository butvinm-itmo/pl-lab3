#ifndef MESSAGES_H
#define MESSAGES_H

static const char
    *MSG_WRONG_USAGE = "Usage: ./image-transformer <input> <output> <angle>";
static const char
    *MSG_INCORRECT_ANGLE = "Incorrect angle format. Should be one of: 0, 90, "
                           "-90, 180, -180, 270, -270";
static const char *MSG_CANNOT_READ = "Cannot read %s.";
static const char
    *MSG_BAD_HEADER = "Bad image header. Are you sure it is BMP file?";
static const char *MSG_UNSUPPORTED_FORMAT =
    "Wrong image format. Are you sure it is BMP file?";
static const char
    *MSG_UNSUPPORTED_COLOR_DEPTH = "Sorry, we support only 24-bit images >_<";
static const char *MSG_UNSUPPORTED_COMPRESSION =
    "Sorry, we support only uncompressed images >_<";
static const char *MSG_BAD_FILE = "Probably, your file is lost or corrupted.";
static const char
    *MSG_LARGE_FILE = "I not sure I can fit so large file (,,>﹏<,,)";
static const char *MSG_BMP_PARSING_ERROR = "Unknown error during BMP parsing.";
static const char *MSG_CANNOT_WRITE = "Cannot write %s.";
static const char *MSG_SUCCESS = "Transformed image successfully saved in %s";

#endif
