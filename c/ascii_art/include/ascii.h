#ifndef ASCII_H
#define ASCII_H

#include "image.h"
#include "stdio.h"
#include <stdint.h>

// ASCII palette ordered from darkest (dense) to lightest (sparse)
extern const char ASCII_PALETTE[];
extern const int ASCII_PALETTE_SIZE;

// Map a grayscale value (0-255) to an ASCII character
char ascii_from_brightness(uint8_t brightness);

// Compute output size in characters, rounding up so edge pixels are kept
void ascii_output_size(const Image *img, int scale_factor, int *width,
                       int *height);

// Smallest scale factor whose output fits in cols x rows characters
int ascii_fit_scale(const Image *img, int cols, int rows);

// Convert an image to ASCII art and print to stdout
// invert maps bright pixels to dense characters (for dark backgrounds)
void ascii_print_image(const Image *img, int scale_factor, int use_color,
                       int invert);

// Convert an image to ASCII art and write to file
int ascii_write_to_file(const Image *img, int scale_factor,
                        const char *filename, int invert);

#endif // ASCII_H
