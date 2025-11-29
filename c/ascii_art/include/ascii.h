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

// Convert an image to ASCII art and print to stdout
void ascii_print_image(const Image *img, int scale_factor, int use_color);

// Convert an image to ASCII art and write to file
int ascii_write_to_file(const Image *img, int scale_factor,
                        const char *filename);

#endif // ASCII_H
