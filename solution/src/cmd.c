#include "cmd.h"

#include "processing/rotation.h"

ArgsParseResult parse_cmd_args(int argc, char const **argv) {
    if (argc < 4) {
        return (ArgsParseResult){CMD_ARGS_NOT_ENOUGH};
    }
    const MaybeRotationAngle angle = parse_rotation_angle(argv[3]);
    if (!angle.status) {
        return (ArgsParseResult){CMD_ARGS_BAD_ANGLE};
    }
    return (ArgsParseResult){
        CMD_ARGS_CORRECT,
        {
            .source_image_path = argv[1],
            .output_image_path = argv[2],
            .angle = angle._,
        },
    };
}
