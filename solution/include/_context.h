#ifndef CONTEXT_H
#define CONTEXT_H

#include "_cmd.h"
#include "image.h"
#include "image_io.h"

/* Application execution context. */
typedef struct {
    ArgsParseResult args;
    ImageOpenResult read_result;
    ImageSaveResult write_result;
    MaybeImage rotated_img;
    Image source_img;
} Context;

/* Utils for context printing. */
#define CONTEXT_FMT                                                            \
    "{\n  args: %d\n  read: %d\n  write: %d\n  rotated: %p\n  source: "        \
    "%p\n}\n"

#define CONTEXT_FMT_ARGS(context)                                              \
    (context).args.status, (context).read_result.status,                       \
        (context).write_result.status, (void *)(context).rotated_img._.pixels, \
        (void *)(context).source_img.pixels
#endif
