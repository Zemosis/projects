#include "../include/ascii.h"
#include "../include/image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.1.0"

int main(int argc, char *argv[]) {
    printf("ASCII Art Converter v%s\n", VERSION);

    // Error messege if argument is wrong
    if (argc < 2) {
        printf("Usage: %s <image-file> [scale] [output-file] [--no-color] "
               "[--invert]\n",
               argv[0]);
        printf("  scale: downsampling factor (default: 4)\n");
        printf("  output-file: optional file to save ASCII art\n");
        printf("               if not specified, prints to console\n");
        printf("  --no-color: disable ANSI color codes\n");
        printf("  --invert: bright pixels get dense characters "
               "(for dark backgrounds)\n");
        printf("\nExamples:\n");
        printf("  %s image.jpg          # Print with default scale (4)\n",
               argv[0]);
        printf("  %s image.jpg 8        # Print with scale 8\n", argv[0]);
        printf("  %s image.jpg 8 --no-color # Print without color\n", argv[0]);
        printf("  %s image.jpg 8 --invert   # Print inverted\n", argv[0]);
        printf("  %s image.jpg 4 out.txt  # Save to file with scale 4\n",
               argv[0]);
        printf("         higher values = smaller output\n");
        return 1;
    }

    // Parse optional scale factor
    int scale_factor = 4; // Default scale
    int first_flag = 2;   // Index of the first argument after the scale
    if (argc >= 3 && strncmp(argv[2], "--", 2) != 0) {
        scale_factor = atoi(argv[2]);
        if (scale_factor < 1) {
            fprintf(stderr, "Error: scale must be >= 1\n");
            return 1;
        }
        first_flag = 3;
    }

    // Check for output file
    const char *output_file = NULL;
    int use_color = 1;
    int invert = 0;

    for (int i = first_flag; i < argc; i++) {
        if (strcmp(argv[i], "--no-color") == 0) {
            use_color = 0;
        } else if (strcmp(argv[i], "--invert") == 0) {
            invert = 1;
        } else if (strncmp(argv[i], "--", 2) == 0) {
            fprintf(stderr, "Error: unknown option '%s'\n", argv[i]);
            return 1;
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
    int out_width, out_height;
    ascii_output_size(img, scale_factor, &out_width, &out_height);
    printf("Output size: %dx%d characters (scale: %d)\n", out_width,
           out_height, scale_factor);

    if (output_file) {
        // Save to file
        printf("Writing ASCII art to: %s\n", output_file);
        if (ascii_write_to_file(img, scale_factor, output_file, invert) == 0) {
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
        ascii_print_image(img, scale_factor, use_color, invert);
    }

    image_free(img);
    return 0;
}
