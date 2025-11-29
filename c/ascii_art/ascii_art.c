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

    // Test grayscale conversion on a few pixels
    printf("\nTesting grayscale conversion on first 3 pixels:\n");
    for (int i = 0; i < 3 && i < img->width * img->height; i++) {
        uint8_t r = img->data[i * 3 + 0];
        uint8_t g = img->data[i * 3 + 1];
        uint8_t b = img->data[i * 3 + 2];
        uint8_t gray = image_rgb_to_gray(r, g, b);
        printf("  Pixel %d: RGB(%3d,%3d,%3d) -> Gray(%3d)\n", i, r, g, b, gray);
    }
    image_free(img);
    return 0;
}
