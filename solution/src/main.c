#include "_cmd.h"
#include "_messages.h"
#include "bmp.h"
#include "image_io.h"
#include "log.h"
#include "processing/rotation.h"
#include "result.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    /* Program done successfully. */
    EXIT_OK = EXIT_SUCCESS,
    /* Error caused by wrong usage. */
    EXIT_WRONG_USAGE,
    /* Error during image reading. */
    EXIT_READ,
    /* Error during processing. */
    EXIT_PROC,
    /* Error during image saving. */
    EXIT_WRITE,
} ExitCodes;

typedef struct {
    ArgsParseResult args;
    ImageOpenResult read_result;
    ImageSaveResult write_result;
    MaybeImage rotated_img;
    Image source_img;
} Context;

#define CONTEXT_FMT                                                            \
    "{\n  args: %d\n  read: %d\n  write: %d\n  rotated: %p\n  source: "        \
    "%p\n}\n"

#define CONTEXT_FMT_ARGS(context)                                              \
    (context).args.status, (context).read_result.status,                       \
        (context).write_result.status, (void *)(context).rotated_img._.pixels, \
        (void *)(context).source_img.pixels

static void _exit(Context *context, int8_t code) {
    DEBUG("Exit and clear resources.");
    destroy_image(context->source_img);
    destroy_image(context->rotated_img._);
    exit(code);
}

static void _handle_cmd_args_parse_error(Context *context) {
    static const char **messages[] = {
        [CMD_ARGS_NOT_ENOUGH] = &MSG_WRONG_USAGE,
        [CMD_ARGS_BAD_ANGLE] = &MSG_INCORRECT_ANGLE,
    };
    if (context->args.status != CMD_ARGS_CORRECT) {
        ERROR(*messages[context->args.status]);
        _exit(context, EXIT_WRONG_USAGE);
    }
}

static void _handle_read_bmp_error(Context *context) {
    static const char **messages[] = {
        [FROM_BMP_INVALID_HEADER] = &MSG_BAD_HEADER,
        [FROM_BMP_UNSUPPORTED_FORMAT] = &MSG_UNSUPPORTED_FORMAT,
        [FROM_BMP_UNSUPPORTED_COLOR_DEPTH] = &MSG_UNSUPPORTED_COLOR_DEPTH,
        [FROM_BMP_UNSUPPORTED_COMPRESSION] = &MSG_UNSUPPORTED_COMPRESSION,
        [FROM_BMP_INVALID_PIXELS] = &MSG_BAD_FILE,
        [FROM_BMP_CANNOT_ALLOC_MEMORY] = &MSG_LARGE_FILE,
    };
    if (context->read_result.status != IO_OK) {
        ERRORF(MSG_CANNOT_READ, context->args._.source_image_path);
        _exit(context, EXIT_READ);
    } else if (context->read_result._.status != FROM_BMP_OK) {
        ERROR(*messages[context->read_result._.status]);
        _exit(context, EXIT_READ);
    }
}

static void _handle_rotation_error(Context *context) {
    if (!context->rotated_img.status) {
        ERROR(MSG_LARGE_FILE);
        _exit(context, EXIT_PROC);
    }
}

static void _handle_write_bmp_error(Context *context) {
    static const char **messages[] = {
        [TO_BMP_WRITE_FAILED] = &MSG_CANNOT_WRITE,
        [TO_BMP_BAD_IMAGE_DATA] = &MSG_BAD_IMAGE_DATA,
    };
    if (context->write_result.status != IO_OK) {
        ERRORF(MSG_CANNOT_WRITE, context->args._.output_image_path);
        _exit(context, EXIT_WRITE);
    } else if (context->write_result._ == TO_BMP_BAD_IMAGE_DATA) {
        ERROR(*messages[context->write_result._]);
        _exit(context, EXIT_WRITE);
    }
}

int main(int argc, char const **argv) {
    LOG_LEVEL = LOG_DEBUG;

    Context context = {0};

    context.args = parse_cmd_args(argc, argv);
    _handle_cmd_args_parse_error(&context); // Possible exit point!
    DEBUGF(CONTEXT_FMT, CONTEXT_FMT_ARGS(context));

    context.read_result = image_open(
        context.args._.source_image_path, from_bmp
    );
    context.source_img = context.read_result._._;
    _handle_read_bmp_error(&context); // Possible exit point!
    DEBUGF(CONTEXT_FMT, CONTEXT_FMT_ARGS(context));

    context.rotated_img = rotate_image(
        context.source_img, context.args._.angle
    );
    _handle_rotation_error(&context);
    DEBUGF(CONTEXT_FMT, CONTEXT_FMT_ARGS(context));

    context.write_result = image_save(
        context.args._.output_image_path, context.rotated_img._, to_bmp
    );
    _handle_write_bmp_error(&context);
    DEBUGF(CONTEXT_FMT, CONTEXT_FMT_ARGS(context));

    INFOF(MSG_SUCCESS, context.args._.output_image_path);
    _exit(&context, EXIT_OK);
    return EXIT_FAILURE; // Unreachable
}
