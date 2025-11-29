#include "ascii.h"
#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.1.0"

int main(int argc, char *argv[]) {
    printf("ASCII Art Converter v%s\n", VERSION);

    // Error messege if argument is wrong
    if (argc < 2) {
        printf("Usage: %s <image-file>\n", argv[0]);
        printf("  scale: downsampling factor (default: 4)\n");
        printf("  output-file: optional file to save ASCII art\n");
        printf("               if not specified, prints to console\n");
        printf("\nExamples:\n");
        printf("  %s image.jpg          # Print with default scale (4)\n",
               argv[0]);
        printf("  %s image.jpg 8        # Print with scale 8\n", argv[0]);
        printf("  %s image.jpg 8 --no-color # Print without color\n", argv[0]);
        printf("  %s image.jpg 4 out.txt  # Save to file with scale 4\n",
               argv[0]);
        printf("         higher values = smaller output\n");
        return 1;
    }

    // Parse optional scale factor
    int scale_factor = 4; // Default scale
    if (argc >= 3 && strcmp(argv[2], "--no-color") != 0) {
        scale_factor = atoi(argv[2]);
        if (scale_factor < 1) {
            fprintf(stderr, "Error: scale must be >= 1\n");
            return 1;
        }
    }

    // Check for output file
    const char *output_file = NULL;
    int use_color = 1;

    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "--no-color") == 0) {
            use_color = 0;
        } else if (!output_file) {
            output_file = argv[i];
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
    printf("Output size: %dx%d characters (scale: %d)\n",
           img->width / scale_factor, img->height / (scale_factor * 2),
           scale_factor);

    if (output_file) {
        // Save to file
        printf("Writing ASCII art to: %s\n", output_file);
        if (ascii_write_to_file(img, scale_factor, output_file) == 0) {
            printf("Successfully saved ASCII art to '%s'\n", output_file);
        } else {
            fprintf(stderr, "Error: Failed to write to file\n");
            image_free(img);
            return 1;
        }
    } else {
        // Print to console
        if (use_color) {
            printf("Color mode: enabled (use --no-color to disable)\n");
        }
        ascii_print_image(img, scale_factor, use_color);
    }

    image_free(img);
    return 0;
}
