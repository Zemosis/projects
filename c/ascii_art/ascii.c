#include "ascii.h"
#include <stdio.h>

// ASCII palette ordered from darkest (dense) to lightest (sparse)
const char ASCII_PALETTE[] =
    "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

const int ASCII_PALETTE_SIZE =
    sizeof(ASCII_PALETTE) - 1; // -1 to exclude null terminator

char ascii_from_brightness(uint8_t brightness) {
    // Map brightness (0-255) to palette index (0 to palette_size-1)
    int index = (brightness * ASCII_PALETTE_SIZE) / 256;

    // Clamp to valid range
    if (index < 0)
        index = 0;
    if (index >= ASCII_PALETTE_SIZE)
        index = ASCII_PALETTE_SIZE - 1;

    return ASCII_PALETTE[index];
}

void ascii_print_image(const Image *img, int scale_factor) {
    if (!img || !img->data || scale_factor < 1) {
        fprintf(stderr, "Invalid image or scale factor\n");
        return;
    }

    int scaled_width = img->width / scale_factor;
    int scaled_height = img->height / scale_factor;

    printf("\nASCII Art (%dx%d, scale: %d):\n", scaled_width, scaled_height,
           scale_factor);
    printf("---\n");

    // Process image row by row
    for (int y = 0; y < scaled_height; y++) {
        for (int x = 0; x < scaled_width; x++) {
            // Sample the pixel at the scaled position
            int src_x = x * scale_factor;
            int src_y = y * scale_factor;

            // Calculate pixel index in the data array
            int pixel_index = (src_y * img->width + src_x) * img->channels;

            // Get RGB values
            uint8_t r = img->data[pixel_index + 0];
            uint8_t g = img->data[pixel_index + 1];
            uint8_t b = img->data[pixel_index + 2];

            // Convert to grayscale
            uint8_t gray = image_rgb_to_gray(r, g, b);

            // Map to ASCII character
            char ascii = ascii_from_brightness(gray);

            // Print character (print twice for better aspect ratio)
            // ASCII characters are taller than they are wide
            printf("%c%c", ascii, ascii);
        }
        printf("\n");
    }
    printf("---\n");
}
