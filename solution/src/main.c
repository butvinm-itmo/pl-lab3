#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint8_t r, g, b;
} pixel;

typedef struct {
    uint32_t width, height;
    pixel* pixels;
} image;

typedef struct __attribute__((packed)) {
    /* The header field used to identify the BMP and DIB file. Set to 0x42 0x4D
     * in hexadecimal, same as BM in ASCII. */
    uint16_t bfType;
    /* The size of the BMP file in bytes. */
    uint32_t bfileSize;
    /* Reserved. */
    uint32_t bfReserved;
    /* Starting address of the byte where the bitmap pixel array starts. */
    uint32_t bOffBits;
    /* The size of this header (12 bytes). */
    uint32_t biSize;
    /* Bitmap width in pixels */
    uint32_t biWidth;
    /* Bitmap height in pixels. */
    uint32_t biHeight;
    /* Number of color planes (must be 1). */
    uint16_t biPlanes;
    /* Number of bits per pixel. Typical values are 1, 4, 8, 16, 24 and 32.. */
    uint16_t biBitCount;
    /* Compression method. */
    uint32_t biCompression;
    /* Image size. This is the size of the raw bitmap data. */
    uint32_t biSizeImage;
    /* Horizontal resolution of the image. (pixel per metre, signed integer). */
    uint32_t biXPelsPerMeter;
    /* Vertical resolution of the image. (pixel per metre, signed integer). */
    uint32_t biYPelsPerMeter;
    /* Number of colors in the color palette, or 0 to default to 2n. */
    uint32_t biClrUsed;
    /* Number of important colors used, or 0 when every color is important */
    uint32_t biClrImportant;
} bmp_header;

/* From BMP specification. */
#define BMP_FILE_TYPE 0x4D42  // Little-endian 'BM'
#define BMP_HEADER_SIZE 40
#define BMP_COMPRESSION_BI_RGB 0
#define BMP_COLOR_DEPTH (sizeof(pixel) * 8)

typedef enum {
    /* IMage read properly. */
    OK = 0,
    /* Unsupported or broken header. */
    INVALID_HEADER,
    /* Not BMP format. */
    UNSUPPORTED_FORMAT,
    /* Unsupported compression. */
    UNSUPPORTED_COMPRESSION,
    /* Unsupported color depth. */
    UNSUPPORTED_COLOR_DEPTH,
    /* Cannot read pixel data. */
    INVALID_PIXELS,
    /* Cannot write to image structure. */
    BAD_IMAGE_PTR,
} read_result;

uint8_t calc_width_padding(size_t row_size) { return row_size % 4; }

read_result validate_signature(bmp_header* header) {
    if (header->bfType != BMP_FILE_TYPE) {
        return UNSUPPORTED_FORMAT;
    }
    if (header->biSize != BMP_HEADER_SIZE) {
        return INVALID_HEADER;
    }
    if (header->biCompression != BMP_COMPRESSION_BI_RGB) {
        return UNSUPPORTED_COMPRESSION;
    }
    if (header->biBitCount != BMP_COLOR_DEPTH) {
        return UNSUPPORTED_COLOR_DEPTH;
    }
    return OK;
}

read_result from_bmp(FILE* in, image* img) {
    if (img == NULL) {
        return BAD_IMAGE_PTR;
    }

    bmp_header header = {0};
    if (fread(&header, sizeof(bmp_header), 1, in) != 1) {
        return INVALID_HEADER;
    }
    read_result signature_result = validate_signature(&header);
    if (signature_result != OK) {
        return signature_result;
    }
    // Set cursor at pixels array.
    if (fseek(in, header.bOffBits, SEEK_SET) != 0) {
        return INVALID_PIXELS;
    }

    const size_t row_size = header.biWidth * sizeof(pixel);
    const uint8_t row_padding = calc_width_padding(row_size);
    pixel* pixels = malloc(row_size * header.biHeight);

    bool read_fail = false;
    for (uint32_t row = 0; row < header.biHeight; row++) {
        if (fread(pixels + row * header.biWidth, row_size, 1, in) != 0) {
            break;
        }
        if (fseek(in, row_padding, SEEK_CUR)) {
            break;
        }
    }
    if (read_fail) {
        free(pixels);
        return INVALID_PIXELS;
    }
    *img = (image
    ){.width = header.biWidth, .height = header.biHeight, .pixels = pixels};
    return OK;
}

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
