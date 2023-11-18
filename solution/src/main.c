#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
#include "cmd.h"
#include "io.h"
#include "log.h"
#include "messages.h"
#include "processing/rotation.h"
#include "result.h"

IO_RESULT(read_bmp_result, FromBmpResult);

read_bmp_result read_bmp(char *img_path) {
    FILE *file = fopen(img_path, "r");
    if (file == NULL) {
        return (read_bmp_result){IO_OPEN_ERR};
    }
    FromBmpResult result = from_bmp(file);
    if (fclose(file) != 0) {
        return (read_bmp_result){IO_CLOSE_ERR};
    }
    return (read_bmp_result){IO_OK, result};
}

IO_RESULT(write_bmp_result, ToBmpStatus);

write_bmp_result write_bmp(char *img_path, Image img) {
    FILE *file = fopen(img_path, "w");
    if (file == NULL) {
        return (write_bmp_result){IO_OPEN_ERR};
    }
    ToBmpStatus status = to_bmp(file, img);
    if (fclose(file) != 0) {
        return (write_bmp_result){IO_CLOSE_ERR};
    }
    return (write_bmp_result){IO_OK, status};
}

int main(int argc, char **argv) {
    LOG_LEVEL = LOG_INFO;

    ArgsParseResult args = parse_cmd_args(argc, argv);
    if (args.status == CMD_ARGS_NOT_ENOUGH) {
        ERROR(MSG_WRONG_USAGE);
        exit(1);
    } else if (args.status == CMD_ARGS_BAD_ANGLE) {
        ERROR(MSG_INCORRECT_ANGLE);
        exit(1);
    }

    read_bmp_result read_result = read_bmp(args._.source_image_path);
    if (read_result.status != IO_OK) {
        ERRORF(MSG_CANNOT_READ, args._.source_image_path);
        exit(1);
    } else if (read_result._.status != FROM_BMP_OK) {
        switch (read_result._.status) {
        case FROM_BMP_INVALID_HEADER:
            ERROR(MSG_BAD_HEADER);
            break;
        case FROM_BMP_UNSUPPORTED_FORMAT:
            ERROR(MSG_UNSUPPORTED_FORMAT);
            break;
        case FROM_BMP_UNSUPPORTED_COLOR_DEPTH:
            ERROR(MSG_UNSUPPORTED_COLOR_DEPTH);
            break;
        case FROM_BMP_UNSUPPORTED_COMPRESSION:
            ERROR(MSG_UNSUPPORTED_COMPRESSION);
            break;
        case FROM_BMP_INVALID_PIXELS:
            ERROR(MSG_BAD_FILE);
            break;
        case FROM_BMP_CANNOT_ALLOC_MEMORY:
            ERROR(MSG_LARGE_FILE);
            break;
        default:
            ERROR(MSG_BMP_PARSING_ERROR);
            break;
        }
        exit(1);
    }

    Image img = read_result._._;
    Image rotated_img = rotate_image(img, args._.angle);

    write_bmp_result write_result = write_bmp(
        args._.output_image_path, rotated_img);
    if (write_result.status != IO_OK || write_result._ != TO_BMP_OK) {
        ERRORF(MSG_CANNOT_WRITE, args._.output_image_path);
        exit(1);
    };

    INFOF(MSG_SUCCESS, args._.output_image_path);

    DEBUG("Clean image data...");
    destroy_image(img);
    destroy_image(rotated_img);

    return 0;
}
