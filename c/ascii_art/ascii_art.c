#include "ascii.h"
#include "image.h"
#include <stdio.h>
#include <stdlib.h>

#define VERSION "0.1.0"

int main(int argc, char *argv[]) {
    printf("ASCII Art Converter v%s\n", VERSION);

    // Error messege if argument is wrong
    if (argc < 2) {
        printf("Usage: %s <image-file>\n", argv[0]);
        printf("  scale: downsampling factor (default: 4)\n");
        printf("         higher values = smaller output\n");
        return 1;
    }

    // Parse optional scale factor
    int scale_factor = 4; // Default scale
    if (argc >= 3) {
        scale_factor = atoi(argv[2]);
        if (scale_factor < 1) {
            fprintf(stderr, "Error: scale must be >= 1\n");
            return 1;
        }
    }

    printf("Loading image: %s\n", argv[1]);
    Image *img = image_load(argv[1]);

    if (!img) {
        fprintf(stderr, "Error: Could not load image\n");
        return 1;
    }

    printf("Successfully loaded: %dx%d (%d channels)\n", img->width,
           img->height, img->channels);
    printf("Output size: %dx%d characters\n", img->width / scale_factor,
           img->height / scale_factor);

    // Convert and print ASCII art
    ascii_print_image(img, scale_factor);

    // // Display the ASCII palette
    // printf("\nASCII Palette (dark to light): '%s'\n", ASCII_PALETTE);
    // printf("Palette size: %d characters\n", ASCII_PALETTE_SIZE);
    //
    // // Test brightness to ASCII conversion
    // printf("\nBrightness to ASCII mapping (every 16 levels):\n");
    // for (int i = 0; i <= 255; i += 16) {
    //     char ascii = ascii_from_brightness(i);
    //     printf("  Brightness %3d -> '%c'\n", i, ascii);
    // }
    //
    // // Test on actual image pixels
    // printf("\nTesting on first 5 pixels:\n");
    // for (int i = 0; i < 5 && i < img->width * img->height; i++) {
    //     uint8_t r = img->data[i * 3 + 0];
    //     uint8_t g = img->data[i * 3 + 1];
    //     uint8_t b = img->data[i * 3 + 2];
    //     uint8_t gray = image_rgb_to_gray(r, g, b);
    //     char ascii = ascii_from_brightness(gray);
    //     printf("  Pixel %d: RGB(%3d,%3d,%3d) -> Gray(%3d) -> '%c'\n", i, r,
    //     g,
    //            b, gray, ascii);
    // }
    image_free(img);
    return 0;
}
