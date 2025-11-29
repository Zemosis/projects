#include "image.h"
#include <stdio.h>

#define VERSION "0.1.0"

int main(int argc, char *argv[]) {
    printf("ASCII Art Converter v%s\n", VERSION);

    if (argc < 2) {
        printf("Usage: %s <image-file>\n", argv[0]);
        return 1;
    }

    printf("Loading image: %s\n", argv[1]);
    Image *img = image_load(argv[1]);

    if (!img) {
        fprintf(stderr, "Error: Could not load image\n");
        return 1;
    }

    printf("Successfully loaded: %dx%d (%d channels)\n", img->width,
           img->height, img->channels);

    image_free(img);
    return 0;
}
