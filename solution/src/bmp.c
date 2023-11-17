#include "bmp.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
#define BMP_FILE_TYPE 0x4D42 // Little-endian 'BM'
#define BMP_HEADER_SIZE 40
#define BMP_COMPRESSION_BI_RGB 0
#define BMP_COLOR_DEPTH (sizeof(pixel) * 8)

uint8_t calc_width_padding(size_t row_size) {
    return row_size % 4;
}

read_result validate_signature(bmp_header *header) {
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

read_result from_bmp(FILE *in, image *img) {
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

    const size_t row_size = header.biWidth * sizeof(pixel);
    const uint8_t row_padding = calc_width_padding(row_size);
    pixel *pixels = malloc(row_size * header.biHeight);

    // Set cursor at pixels array.
    if (fseek(in, header.bOffBits, SEEK_SET) != 0) {
        goto invalid_pixels;
    }
    for (uint32_t row = 0; row < header.biHeight; row++) {
        if (fread(pixels + row * header.biWidth, row_size, 1, in) != 1) {
            goto invalid_pixels;
        }
        if (fseek(in, row_padding, SEEK_CUR) != 0) {
            goto invalid_pixels;
        }
    }
    *img = (image){
        .width = header.biWidth, .height = header.biHeight, .pixels = pixels};
    return OK;

invalid_pixels:
    free(pixels);
    return INVALID_PIXELS;
}
