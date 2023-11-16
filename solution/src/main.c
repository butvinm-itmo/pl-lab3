#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;  // supress 'unused parameters' warning

    if (argc < 2) {
        fprintf(
            stderr,
            "Image path missed.\nUsage: ./image-transformer <source-image> "
            "<transformed-image> <angle>\n"
        );
    }

    image img = {0};
    FILE* in = fopen(argv[1], "r");
    read_result result = from_bmp(in, &img);
    printf("%d\n", result);
    fclose(in);
    printf("width: %" PRId32 " height: %" PRId32 "\n", img.width, img.height);
    return 0;
}
