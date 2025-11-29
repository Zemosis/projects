#include "ascii.h"

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
