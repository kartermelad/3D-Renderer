#include "core/pixel_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

PixelBuffer* create_pixel_buffer(int width, int height) {
    PixelBuffer* buffer = malloc(sizeof(PixelBuffer));
    if (!buffer) {
        return NULL;
    }

    buffer->width = width;
    buffer->height = height;
    buffer->pixels = calloc(width * height, sizeof(Color));

    return buffer;
}

void destroy_pixel_buffer(PixelBuffer* buffer) {
    if (!buffer) {
        return;
    }

    free(buffer->pixels);
    free(buffer);
}

void clear_buffer(PixelBuffer* buffer, Color clear_color) {
    if (!buffer) {
        return;
    }

    for (int i = 0; i < buffer->width; i++) {
        for (int j = 0; j < buffer->height; j++) {
            int index = j * buffer->width + i;
            buffer->pixels[index] = clear_color;
        }
    }
}

void set_pixel(PixelBuffer* buffer, int x, int y, Color color) {
    if (!buffer) {
        return;
    }

    if (x < 0 || x >= buffer->width) {
        return;
    }

    if (y < 0 || y >= buffer->height) {
        return;
    }

    int index = y * buffer->width + x;
    buffer->pixels[index] = color;
}

Color get_pixel(PixelBuffer* buffer, int x, int y) {
    if (!buffer) {
        Color black = {0, 0, 0, 0};
        return black;    }

    if (x < 0 || x >= buffer->width) {
        Color black = {0, 0, 0, 0};
        return black;
    }

    if (y < 0 || y >= buffer->height) {
        Color black = {0, 0, 0, 0};
        return black;
    }

    int index = y * buffer->width + x;
    return buffer->pixels[index];
}

void save_to_ppm(PixelBuffer* buffer, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "P3\n%d %d\n255\n", buffer->width, buffer->height);

    for (int y = 0; y < buffer->height; y++) {
        for (int x = 0; x < buffer->width; x++) {
            Color pixel = buffer->pixels[y * buffer->width + x];
            fprintf(file, "%d %d %d ", pixel.r, pixel.g, pixel.b);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}