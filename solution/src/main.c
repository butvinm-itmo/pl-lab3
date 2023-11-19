#include "bmp.h"
#include "cmd.h"
#include "io.h"
#include "log.h"
#include "messages.h"
#include "processing/rotation.h"
#include "result.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

IO_RESULT(ReadBmpResult, FromBmpResult);

static ReadBmpResult read_bmp(char const *img_path) {
    FILE *file = fopen(img_path, "rb");
    if (file == NULL) {
        return (ReadBmpResult){IO_OPEN_ERR};
    }
    const FromBmpResult result = from_bmp(file);
    if (fclose(file) != 0) {
        return (ReadBmpResult){IO_CLOSE_ERR};
    }
    return (ReadBmpResult){IO_OK, result};
}

IO_RESULT(WriteBmpResult, ToBmpStatus);

static WriteBmpResult write_bmp(char const *img_path, Image img) {
    FILE *file = fopen(img_path, "wb");
    if (file == NULL) {
        return (WriteBmpResult){IO_OPEN_ERR};
    }
    const ToBmpStatus status = to_bmp(file, img);
    if (fclose(file) != 0) {
        return (WriteBmpResult){IO_CLOSE_ERR};
    }
    return (WriteBmpResult){IO_OK, status};
}

int main(int argc, char const **argv) {
    LOG_LEVEL = LOG_INFO;

    const ArgsParseResult args = parse_cmd_args(argc, argv);
    if (args.status == CMD_ARGS_NOT_ENOUGH) {
        ERROR(MSG_WRONG_USAGE);
        exit(1);

    } else if (args.status == CMD_ARGS_BAD_ANGLE) {
        ERROR(MSG_INCORRECT_ANGLE);
        exit(2);
    }

    const ReadBmpResult read_result = read_bmp(args._.source_image_path);
    if (read_result.status != IO_OK) {
        ERRORF(MSG_CANNOT_READ, args._.source_image_path);
        exit(3);

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
        exit(4);
    }

    Image img = read_result._._;
    MaybeImage rotated_img = rotate_image(img, args._.angle);
    if (!rotated_img.status) {
        ERROR(MSG_LARGE_FILE);
        destroy_image(img);
        destroy_image(rotated_img._);
        exit(5);
    }

    const WriteBmpResult write_result = write_bmp(
        args._.output_image_path, rotated_img._
    );
    if (write_result.status != IO_OK || write_result._ == TO_BMP_WRITE_FAILED) {
        ERRORF(MSG_CANNOT_WRITE, args._.output_image_path);
        destroy_image(img);
        destroy_image(rotated_img._);
        exit(6);
    } else if (write_result._ == TO_BMP_BAD_IMAGE_DATA) {
        ERROR(MSG_BAD_IMAGE_DATA);
        destroy_image(img);
        destroy_image(rotated_img._);
        exit(6);
    }

    INFOF(MSG_SUCCESS, args._.output_image_path);

    destroy_image(img);
    destroy_image(rotated_img._);
    return 0;
}
