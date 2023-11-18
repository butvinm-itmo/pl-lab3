#ifndef CMD_H
#define CMD_H

#include "processing/rotation.h"
#include "result.h"

/* Command line args required for program. */
typedef struct {
    char *source_image_path;
    char *output_image_path;
    RotationAngle angle;
} CmdArgs;

/* Status of command line args parsing. */
typedef enum {
    /* Command args parsed correctly. */
    CMD_ARGS_CORRECT = 0,
    /* Not enough command arguments. */
    CMD_ARGS_NOT_ENOUGH,
    /* Incorrect angle value. */
    CMD_ARGS_BAD_ANGLE,
} ArgsParseStatus;

Result(ArgsParseResult, CmdArgs, ArgsParseStatus);

/* Parse command line args. */
ArgsParseResult parse_cmd_args(int argc, char **argv);

#endif
