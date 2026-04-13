#include "../include/image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

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
    memcpy(img->data, data, (size_t)width * height * 3);

    stbi_image_free(data);
    return img;
}

uint8_t image_rgb_to_gray(uint8_t r, uint8_t g, uint8_t b) {
    // Use luminosity method (weighted average)
    // Standard weights: R: 0.299, G: 0.587, B: 0.114
    int gray = (299 * r + 587 * g + 114 * b) / 1000;
    return (uint8_t)gray;
}

Image *image_downscale(const Image *img, int target_w, int target_h) {
    if (!img || !img->data || target_w < 1 || target_h < 1) {
        return NULL;
    }

    Image *out = image_create(target_w, target_h, img->channels);
    if (!out) {
        return NULL;
    }

    for (int y = 0; y < target_h; y++) {
        for (int x = 0; x < target_w; x++) {
            int src_x0 = x * img->width / target_w;
            int src_y0 = y * img->height / target_h;
            int src_x1 = (x + 1) * img->width / target_w;
            int src_y1 = (y + 1) * img->height / target_h;

            int total_r = 0, total_g = 0, total_b = 0;
            int count = 0;

            for (int sy = src_y0; sy < src_y1; sy++) {
                for (int sx = src_x0; sx < src_x1; sx++) {
                    int idx = (sy * img->width + sx) * img->channels;
                    total_r += img->data[idx + 0];
                    total_g += img->data[idx + 1];
                    total_b += img->data[idx + 2];
                    count++;
                }
            }

            int out_idx = (y * target_w + x) * img->channels;
            if (count > 0) {
                out->data[out_idx + 0] = (uint8_t)(total_r / count);
                out->data[out_idx + 1] = (uint8_t)(total_g / count);
                out->data[out_idx + 2] = (uint8_t)(total_b / count);
            } else {
                out->data[out_idx + 0] = 0;
                out->data[out_idx + 1] = 0;
                out->data[out_idx + 2] = 0;
            }
        }
    }

    return out;
}
