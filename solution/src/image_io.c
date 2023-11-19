#include "image_io.h"

#include <stdio.h>

ImageOpenResult image_open(char const *img_path, ImageReader reader) {
    FILE *file = fopen(img_path, "rb");
    if (file == NULL) {
        return (ImageOpenResult){IO_OPEN_ERR};
    }
    const ImageReaderResult result = reader(file);
    if (fclose(file) != 0) {
        return (ImageOpenResult){IO_CLOSE_ERR};
    }
    return (ImageOpenResult){IO_OK, result};
}

ImageSaveResult image_save(
    char const *img_path,
    Image img,
    ImageWriter writer
) {
    FILE *file = fopen(img_path, "wb");
    if (file == NULL) {
        return (ImageSaveResult){IO_OPEN_ERR};
    }
    const ImageWriterResult result = writer(file, img);
    if (fclose(file) != 0) {
        return (ImageSaveResult){IO_CLOSE_ERR};
    }
    return (ImageSaveResult){IO_OK, result};
}
