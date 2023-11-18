#include "cmd.h"

cmd_args_result parse_cmd_args(int argc, char **argv) {
    if (argc < 4) {
        return (cmd_args_result){CMD_ARGS_NOT_ENOUGH};
    }
    angle_parse_result angle = parse_rot_angle(argv[3]);
    if (!angle.status) {
        return (cmd_args_result){CMD_ARGS_BAD_ANGLE};
    }
    return (cmd_args_result){
        CMD_ARGS_CORRECT,
        {
            .source_image_path = argv[1],
            .output_image_path = argv[2],
            .angle = angle.value,
        },
    };
}
