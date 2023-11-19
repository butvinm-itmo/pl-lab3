#ifndef IMAGE_IO
#define IMAGE_IO

#include "image.h"
#include "result.h"

#include <stdio.h>

/* Image reader result. */
Result(ImageReaderResult, Image, uint64_t);

/* Result of image opening. */
IOResult(ImageOpenResult, ImageReaderResult);

/* Function that read image file and convert to specific format. */
typedef ImageReaderResult (*ImageReader)(FILE *file);

/* Read and parse image. */
ImageOpenResult image_open(char const *img_path, ImageReader reader);

/* Image writer result. */
typedef uint64_t ImageWriterResult;

/* Result of image building and saving. */
IOResult(ImageSaveResult, ImageWriterResult);

/* Function that read image file and convert to specific format. */
typedef ImageWriterResult (*ImageWriter)(FILE *file, Image img);

/* Build and save image. */
ImageSaveResult image_save(char const *img_path, Image img, ImageWriter writer);

#endif
