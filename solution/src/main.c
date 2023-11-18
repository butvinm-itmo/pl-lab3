#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
#include "cmd.h"
#include "io.h"
#include "processing/rotation.h"
#include "result.h"

IO_RESULT(read_bmp_result, from_bmp_result)

read_bmp_result read_bmp(char *image_path) {
    FILE *file = fopen(image_path, "r");
    if (file == NULL) {
        return (read_bmp_result){IO_OPEN_ERR};
    }
    from_bmp_result result = from_bmp(file);
    if (fclose(file) != 0) {
        return (read_bmp_result){IO_CLOSE_ERR};
    }
    return (read_bmp_result){IO_OK, result};
}

IO_RESULT(write_bmp_result, to_bmp_status)

write_bmp_result write_bmp(char *image_path, image img) {
    FILE *file = fopen(image_path, "w");
    if (file == NULL) {
        return (write_bmp_result){IO_OPEN_ERR};
    }
    to_bmp_status status = to_bmp(file, img);
    if (fclose(file) != 0) {
        return (write_bmp_result){IO_CLOSE_ERR};
    }
    return (write_bmp_result){IO_OK, status};
}

int main(int argc, char **argv) {
    cmd_args_result cmd_args = parse_cmd_args(argc, argv);

    read_bmp_result r_result = read_bmp(cmd_args.value.source_image_path);
    image img = r_result.value.value;
    image rotated_img = rotate_image(img, cmd_args.value.angle);
    write_bmp_result w_result = write_bmp(
        cmd_args.value.output_image_path, rotated_img);

    free(img.pixels);
    free(rotated_img.pixels);

    return r_result.status || w_result.status;
}
