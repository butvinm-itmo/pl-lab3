#include "bmp.h"

#include <inttypes.h>
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
} BmpHeader;

/* From BMP specification. */
BmpHeader _HEADER = {0};
#define BMP_FILE_TYPE 0x4D42              // Little-endian 'BM'
#define BMP_RESERVED 0                    // Always 0
#define BMP_HEADER_SIZE sizeof(_HEADER)   // size of full header
#define BI_HEADER_SIZE sizeof(_HEADER.bi) // 40 bytes
#define BI_RGB_COMPRESSION 0              // We support only uncompressed files
#define BI_PLANES 1                       // Always 1
#define BI_ALL_COLORS 0                   // Enable full color palette
#define BI_DUMMY_SIZE 0              // Image size for not-compressed images
#define PIXEL_SIZE sizeof(Pixel)     // Pixel size in bytes
#define COLOR_DEPTH (PIXEL_SIZE * 8) // 24 bits
#define DPI_72 2835                  // Default image resolution
const char PADDING_BYTES[4];         // Some data to fill paddings

uint8_t _calc_padding(size_t row_size) {
    return (4 - row_size % 4) % 4;
}

FromBmpStatus _validate_signature(BmpHeader *header) {
    if (header->bf.type != BMP_FILE_TYPE) {
        return FROM_BMP_UNSUPPORTED_FORMAT;
    }
    if (header->bi.header_size != BI_HEADER_SIZE) {
        return FROM_BMP_UNSUPPORTED_FORMAT;
    }
    if (header->bi.compression != BI_RGB_COMPRESSION) {
        return FROM_BMP_UNSUPPORTED_COMPRESSION;
    }
    if (header->bi.bit_count != COLOR_DEPTH) {
        return FROM_BMP_UNSUPPORTED_COLOR_DEPTH;
    }
    return FROM_BMP_OK;
}

FromBmpResult from_bmp(FILE *in) {
    BmpHeader header;
    if (fread(&header, sizeof(header), 1, in) != 1) {
        return (FromBmpResult){FROM_BMP_INVALID_HEADER};
    }

    FromBmpStatus signature_status = _validate_signature(&header);
    if (signature_status != FROM_BMP_OK) {
        return (FromBmpResult){signature_status};
    }

    const uint8_t padding = _calc_padding(header.bi.width * PIXEL_SIZE);

    Image img = create_image(header.bi.width, header.bi.height);
    if (img.pixels == NULL) {
        return (FromBmpResult){FROM_BMP_CANNOT_ALLOC_MEMORY};
    }

    // Set cursor at pixels array.
    if (fseek(in, header.bf.pixel_array_offset, SEEK_SET) != 0) {
        return (FromBmpResult){FROM_BMP_INVALID_PIXELS};
    }
    Pixel *row_ptr = img.pixels;
    for (uint32_t row = 0; row < header.bi.height; row++) {
        const size_t pixels_read = fread(
            row_ptr, PIXEL_SIZE, header.bi.width, in);
        if (pixels_read != header.bi.width) {
            return (FromBmpResult){FROM_BMP_INVALID_PIXELS};
        }
        if (fseek(in, padding, SEEK_CUR) != 0) {
            return (FromBmpResult){FROM_BMP_INVALID_PIXELS};
        }
        row_ptr += header.bi.width;
    }
    return (FromBmpResult){FROM_BMP_OK, img};
}

size_t _calc_file_size(uint32_t width, uint32_t height, uint32_t padding) {
    return BMP_HEADER_SIZE + (width * PIXEL_SIZE + padding) * height;
}

ToBmpStatus to_bmp(FILE *out, const Image img) {
    const uint8_t padding = _calc_padding(img.width * PIXEL_SIZE);
    const size_t file_size = _calc_file_size(img.width, img.height, padding);
    BmpHeader header = {
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
                .width = img.width,
                .height = img.height,
                .planes = BI_PLANES,
                .bit_count = COLOR_DEPTH,
                .image_size = BI_DUMMY_SIZE,
                .x_pix_per_meter = DPI_72,
                .y_pix_per_meter = DPI_72,
                .colors_used = BI_ALL_COLORS,
                .colors_important = BI_ALL_COLORS,
            },
    };
    if (fwrite(&header, header.bf.pixel_array_offset, 1, out) != 1) {
        return TO_BMP_FAILED;
    }
    Pixel *row_ptr = img.pixels;
    for (uint32_t row = 0; row < header.bi.height; row++) {
        const size_t pixels_written = fwrite(
            row_ptr, PIXEL_SIZE, header.bi.width, out);
        if (pixels_written != header.bi.width) {
            return TO_BMP_FAILED;
        }
        if (fwrite(PADDING_BYTES, padding, 1, out) != 1) {
            return TO_BMP_FAILED;
        }
        row_ptr += header.bi.width;
    }
    return TO_BMP_OK;
}
