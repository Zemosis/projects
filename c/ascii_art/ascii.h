#ifndef ASCII_H
#define ASCII_H

#include <stdint.h>

// ASCII palette ordered from darkest (dense) to lightest (sparse)
extern const char ASCII_PALETTE[];
extern const int ASCII_PALETTE_SIZE;

// Map a grayscale value (0-255) to an ASCII character
char ascii_from_brightness(uint8_t brightness);

#endif // ASCII_H
