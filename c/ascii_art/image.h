#ifndef IMAGE_H
#define IMAGE_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
        int width;
        int height;
        int channels;  // typically 3 for RGB, 4 for RGBA
        uint8_t *data; // pixel data: width * height * channels bytes
} Image;

/**
 * Create an empty image structure (allocates memory for pixels).
 * Returns NULL on failure.
 */
Image *image_create(int width, int height, int channels);

/**
 * Free an image and its pixel data.
 */
void image_free(Image *img);

// Load an image from file
Image *image_load(const char *filename);

#endif // IMAGE_H
