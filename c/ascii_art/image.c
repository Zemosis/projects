#include "image.h"
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image *image_create(int width, int height, int channels) {
    Image *img = (Image *)malloc(sizeof(Image));
    if (!img)
        return NULL;

    img->width = width;
    img->height = height;
    img->channels = channels;
    img->data = (uint8_t *)malloc(width * height * channels);

    if (!img->data) {
        free(img);
        return NULL;
    }

    return img;
}

void image_free(Image *img) {
    if (img) {
        if (img->data) {
            free(img->data);
        }
        free(img);
    }
}

Image *image_load(const char *filename) {
    int width, height, channels;

    // Load image using stb_image
    uint8_t *data = stbi_load(filename, &width, &height, &channels, 3);

    if (!data) {
        fprintf(stderr, "Failed to load image: %s\n", stbi_failure_reason());
        return NULL;
    }

    // Create our Image structure
    Image *img = image_create(width, height, 3);
    if (!img) {
        stbi_image_free(data);
        return NULL;
    }

    // Copy data (stb_image owns the original buffer)
    for (int i = 0; i < width * height * 3; i++) {
        img->data[i] = data[i];
    }

    stbi_image_free(data);
    return img;
}
