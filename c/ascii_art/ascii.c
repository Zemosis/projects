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

    int char_aspect_ratio = 2;

    int scaled_width = img->width / scale_factor;
    int scaled_height = img->height / scale_factor;

    printf("\nASCII Art (%dx%d, scale: %d):\n", scaled_width, scaled_height,
           scale_factor);
    printf("---\n");

    // Process image row by row
    for (int y = 0; y < scaled_height; y++) {
        for (int x = 0; x < scaled_width; x++) {
            // Average multiple pixels in the block for better quality
            int total_r = 0, total_g = 0, total_b = 0;
            int sample_count = 0;

            // Sample a block of pixels (scale_factor x scale_factor*2)
            for (int dy = 0; dy < scale_factor * char_aspect_ratio; dy++) {
                for (int dx = 0; dx < scale_factor; dx++) {
                    int src_x = x * scale_factor + dx;
                    int src_y = y * scale_factor * char_aspect_ratio + dy;

                    // Bounds check
                    if (src_x < img->width && src_y < img->height) {
                        int pixel_index =
                            (src_y * img->width + src_x) * img->channels;

                        total_r += img->data[pixel_index + 0];
                        total_g += img->data[pixel_index + 1];
                        total_b += img->data[pixel_index + 2];
                        sample_count++;
                    }
                }
            }

            // Calculate average RGB values
            if (sample_count > 0) {
                uint8_t avg_r = total_r / sample_count;
                uint8_t avg_g = total_g / sample_count;
                uint8_t avg_b = total_b / sample_count;

                // Convert to grayscale
                uint8_t gray = image_rgb_to_gray(avg_r, avg_g, avg_b);

                // Map to ASCII character
                char ascii = ascii_from_brightness(gray);

                // Print character once (no doubling needed now)
                printf("%c", ascii);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("---\n");
}
