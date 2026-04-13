#include "../include/ascii.h"
#include <stdio.h>

// ASCII palette ordered from darkest (dense) to lightest (sparse)
const char ASCII_PALETTE[] = "@%#*+=:-. ";

const int ASCII_PALETTE_SIZE =
    sizeof(ASCII_PALETTE) - 1; // -1 to exclude null terminator

char ascii_from_brightness(uint8_t brightness) {
    // Map brightness (0-255) to palette index (0 to palette_size-1)
    int index = (brightness * ASCII_PALETTE_SIZE) / 256;

    // Clamp to valid range
    if (index < 0) {
        index = 0;
    }
    if (index >= ASCII_PALETTE_SIZE) {
        index = ASCII_PALETTE_SIZE - 1;
    }

    return ASCII_PALETTE[index];
}

static void ascii_generate(const Image *img, FILE *output, int use_color) {
    if (!img || !img->data) {
        fprintf(stderr, "Invalid image\n");
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int idx = (y * img->width + x) * img->channels;
            uint8_t r = img->data[idx + 0];
            uint8_t g = img->data[idx + 1];
            uint8_t b = img->data[idx + 2];

            uint8_t gray = image_rgb_to_gray(r, g, b);
            char ascii = ascii_from_brightness(gray);

            if (use_color) {
                fprintf(output, "\033[38;2;%d;%d;%dm%c", r, g, b, ascii);
            } else {
                fprintf(output, "%c", ascii);
            }
        }

        if (use_color) {
            fprintf(output, "\033[0m");
        }
        fprintf(output, "\n");
    }

    if (use_color) {
        fprintf(output, "\033[0m");
    }
}

void ascii_print_image(const Image *img, int use_color) {
    if (!img || !img->data) {
        fprintf(stderr, "Invalid image\n");
        return;
    }

    printf("\nASCII Art (%dx%d%s):\n", img->width, img->height,
           use_color ? ", colored" : "");
    printf("---\n");

    ascii_generate(img, stdout, use_color);

    printf("---\n");
}

int ascii_write_to_file(const Image *img, const char *filename) {
    if (!img || !img->data || !filename) {
        fprintf(stderr, "Invalid parameters for file output\n");
        return -1;
    }

    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s' for writing\n",
                filename);
        return -1;
    }

    ascii_generate(img, file, 0);

    fclose(file);
    return 0;
}
