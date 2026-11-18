#define _POSIX_C_SOURCE 200809L // For ioctl and STDOUT_FILENO under -std=c99

#include "../include/ascii.h"
#include "../include/image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#define VERSION "0.1.0"
#define DEFAULT_SCALE 4

// Lines printed around the art (info lines, header, separators, prompt)
#define RESERVED_LINES 12

// Prototype
static int terminal_size(int *cols, int *rows);

// Get the terminal size in characters, returns 0 on success
static int terminal_size(int *cols, int *rows) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {
        return -1;
    }
    *cols = info.srWindow.Right - info.srWindow.Left + 1;
    *rows = info.srWindow.Bottom - info.srWindow.Top + 1;
#else
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0 || ws.ws_col == 0) {
        return -1;
    }
    *cols = ws.ws_col;
    *rows = ws.ws_row;
#endif
    return 0;
}

int main(int argc, char *argv[]) {
    printf("ASCII Art Converter v%s\n", VERSION);

    // Error messege if argument is wrong
    if (argc < 2) {
        printf("Usage: %s <image-file> [scale] [output-file] [--no-color] "
               "[--invert]\n",
               argv[0]);
        printf("  scale: downsampling factor (default: fit terminal, or 4)\n");
        printf("  output-file: optional file to save ASCII art\n");
        printf("               if not specified, prints to console\n");
        printf("  --no-color: disable ANSI color codes\n");
        printf("  --invert: bright pixels get dense characters "
               "(for dark backgrounds)\n");
        printf("\nExamples:\n");
        printf("  %s image.jpg          # Print scaled to fit the terminal\n",
               argv[0]);
        printf("  %s image.jpg 8        # Print with scale 8\n", argv[0]);
        printf("  %s image.jpg 8 --no-color # Print without color\n", argv[0]);
        printf("  %s image.jpg 8 --invert   # Print inverted\n", argv[0]);
        printf("  %s image.jpg 4 out.txt  # Save to file with scale 4\n",
               argv[0]);
        printf("         higher values = smaller output\n");
        return 1;
    }

    // Parse optional scale factor, only if the argument is a number
    int scale_factor = 0; // 0 means not given, picked after loading
    int first_flag = 2;   // Index of the first argument after the scale
    if (argc >= 3) {
        char *end;
        long scale = strtol(argv[2], &end, 10);
        if (end != argv[2] && *end == '\0') {
            if (scale < 1 || scale > 100000) {
                fprintf(stderr, "Error: scale must be between 1 and 100000\n");
                return 1;
            }
            scale_factor = (int)scale;
            first_flag = 3;
        }
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

    // No scale given: fit the terminal when printing, else use the default
    int cols, rows;
    if (scale_factor == 0) {
        if (!output_file && terminal_size(&cols, &rows) == 0) {
            scale_factor = ascii_fit_scale(img, cols, rows - RESERVED_LINES);
            printf("Fitting to terminal: %dx%d\n", cols, rows);
        } else {
            scale_factor = DEFAULT_SCALE;
        }
    }
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
