#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct __attribute__((packed)) {
    /* The header field used to identify the BMP and DIB file. Set to 0x42 0x4D
     * in hexadecimal, same as BM in ASCII. */
    char bfType[2];
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

typedef struct {
    uint8_t r, g, b;
} pixel;

typedef struct {
    uint32_t width, height;
    pixel* pixels;
} image;

uint32_t get_pixels_array_size(
    uint32_t pixels_width,
    uint32_t pixels_height,
    uint32_t pixel_size
) {
    const uint32_t bytes_width = pixels_width * pixel_size;
    return (bytes_width + bytes_width % 4) * pixels_height;
}

void from_bmp(FILE* in, image* img) {
    bmp_header* header = malloc(sizeof(bmp_header));
    fread(header, sizeof(bmp_header), 1, in);

    // Drop offset bits.
    fseek(in, header->bOffBits, SEEK_SET);

    const size_t pixels_array_size =
        get_pixels_array_size(header->biWidth, header->biHeight, sizeof(pixel));
    pixel* pixels_array = malloc(pixels_array_size);
    fread(pixels_array, pixels_array_size, 1, in);

    if (img != NULL) {
        *img = (image
        ){.width = header->biWidth,
          .height = header->biHeight,
          .pixels = pixels_array};
    }
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
    from_bmp(in, &img);
    fclose(in);
    printf("width: %" PRId32 " height: %" PRId32 "\n", img.width, img.height);
    return 0;
}
