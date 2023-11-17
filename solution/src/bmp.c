#include "bmp.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct __attribute__((packed)) {
    /* Bitmap file header. */
    struct __attribute__((packed)) {
        /* The header field used to identify the BMP and DIB file. Set to 0x42
         * 0x4D in hexadecimal, same as BM in ASCII. */
        uint16_t type;
        /* The size of the BMP file in bytes. */
        uint32_t file_size;
        /* Reserved. */
        uint32_t reserved;
        /* Starting address of the byte where the bitmap pixel array starts. */
        uint32_t pixel_array_offset;
    } bf;
    /* Bitmap information header (Windows BITMAPINFOHEADER). */
    struct __attribute__((packed)) {
        /* The size of this header (40 bytes). */
        uint32_t header_size;
        /* Bitmap width in pixels */
        uint32_t width;
        /* Bitmap height in pixels. */
        uint32_t height;
        /* Number of color planes (must be 1). */
        uint16_t planes;
        /* Number of bits per pixel. */
        uint16_t bit_count;
        /* Compression method. */
        uint32_t compression;
        /* Image size. This is the size of the raw bitmap data. */
        uint32_t image_size;
        /* Horizontal resolution of the image. */
        uint32_t x_pix_per_meter;
        /* Vertical resolution of the image. */
        uint32_t y_pix_per_meter;
        /* Number of colors in the color palette, or 0 to default to 2n. */
        uint32_t colors_used;
        /* Number of important colors, 0 when every color is important. */
        uint32_t colors_important;
    } bi;
} bmp_header;

/* From BMP specification. */
bmp_header _HEADER = {0};
#define BMP_FILE_TYPE 0x4D42              // Little-endian 'BM'
#define BMP_RESERVED 0                    // Always 0
#define BMP_HEADER_SIZE sizeof(_HEADER)   // size of full header
#define BI_HEADER_SIZE sizeof(_HEADER.bi) // 40 bytes
#define BI_RGB_COMPRESSION 0              // We support only uncompressed files
#define BI_PLANES 1                       // Always 1
#define BI_ALL_COLORS 0                   // Enable full color palette
#define BI_DUMMY_SIZE 0              // Image size for not-compressed images
#define PIXEL_SIZE sizeof(pixel)     // Pixel size in bytes
#define COLOR_DEPTH (PIXEL_SIZE * 8) // 24 bits
#define DPI_72 2835                  // Default image resolution

uint8_t calc_width_padding(size_t row_size) {
    return row_size % 4;
}

read_result validate_signature(bmp_header *header) {
    if (header->bf.type != BMP_FILE_TYPE) {
        return UNSUPPORTED_FORMAT;
    }
    if (header->bi.header_size != BI_HEADER_SIZE) {
        return UNSUPPORTED_FORMAT;
    }
    if (header->bi.compression != BI_RGB_COMPRESSION) {
        return UNSUPPORTED_COMPRESSION;
    }
    if (header->bi.bit_count != COLOR_DEPTH) {
        return UNSUPPORTED_COLOR_DEPTH;
    }
    return OK;
}

read_result from_bmp(FILE *in, image *img) {
    if (img == NULL) {
        return BAD_IMAGE_PTR;
    }

    bmp_header header = {0};
    if (fread(&header, sizeof(header), 1, in) != 1) {
        return INVALID_HEADER;
    }

    read_result signature_result = validate_signature(&header);
    if (signature_result != OK) {
        return signature_result;
    }

    const size_t row_size = header.bi.width * sizeof(pixel);
    const uint8_t row_padding = calc_width_padding(row_size);
    pixel *pixels = malloc(row_size * header.bi.height);
    if (pixels == NULL) {
        return CANNOT_ALLOC_MEMORY;
    }

    // Set cursor at pixels array.
    if (fseek(in, header.bf.pixel_array_offset, SEEK_SET) != 0) {
        goto invalid_pixels;
    }
    for (uint32_t row = 0; row < header.bi.height; row++) {
        if (fread(pixels + row * header.bi.width, row_size, 1, in) != 1) {
            goto invalid_pixels;
        }
        if (fseek(in, row_padding, SEEK_CUR) != 0) {
            goto invalid_pixels;
        }
    }
    *img = (image){
        .width = header.bi.width, .height = header.bi.height, .pixels = pixels};
    return OK;

invalid_pixels:
    free(pixels);
    return INVALID_PIXELS;
}

size_t calc_file_size(uint32_t width, uint32_t height) {
    return BMP_HEADER_SIZE + width * height * PIXEL_SIZE;
}

void to_bmp(FILE *out, image *img) {
    const size_t file_size = calc_file_size(img->width, img->height);
    bmp_header header = {
        .bf =
            {
                .type = BMP_FILE_TYPE,
                .file_size = file_size,
                .reserved = BMP_RESERVED,
                .pixel_array_offset = BMP_HEADER_SIZE,
            },
        .bi =
            {
                .header_size = BI_HEADER_SIZE,
                .width = img->width,
                .height = img->height,
                .planes = BI_PLANES,
                .bit_count = COLOR_DEPTH,
                .image_size = BI_DUMMY_SIZE,
                .x_pix_per_meter = DPI_72,
                .y_pix_per_meter = DPI_72,
                .colors_used = BI_ALL_COLORS,
                .colors_important = BI_ALL_COLORS,
            },
    };
    fwrite(&header, BMP_HEADER_SIZE, 1, out);
    printf("%lo ", ftell(out));

    const size_t row_size = img->width * sizeof(pixel);
    const uint8_t row_padding = calc_width_padding(row_size);
    for (uint32_t row = 0; row < img->height; row++) {
        printf("%lo ", ftell(out));
        fwrite(img->pixels + row * img->width, row_size, 1, out);
        fseek(out, row_padding, SEEK_CUR);
    }
}
