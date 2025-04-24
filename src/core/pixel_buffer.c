#include "core/pixel_buffer.h"
#include <stdlib.h>
#include <string.h>

PixelBuffer* create_pixel_buffer(int width, int height) {
    PixelBuffer* buffer = malloc(sizeof(PixelBuffer));
    if (!buffer) {
        return NULL;
    }

    buffer->width = width;
    buffer->height = height;
    buffer->data = calloc(width * height, sizeof(Color));

    return buffer;
}

void destroy_pixel_buffer(PixelBuffer* buffer) {
    if (!buffer) {
        return;
    }

    free(buffer->data);
    free(buffer);
}

void clear_buffer(PixelBuffer* buffer, Color clear_color) {
    if (!buffer) {
        return;
    }

    for (int i = 0; i < buffer->width; i++) {
        for (int j = 0; j < buffer->height; j++) {
            int index = j * buffer->width + i;
            buffer->data[index] = clear_color;
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
    buffer->data[index] = color;
}

Color get_pixel(PixelBuffer* buffer, int x, int y) {
    if (!buffer) {
        Color black = {0, 0, 0};
        return black;    }

    if (x < 0 || x >= buffer->width) {
        Color black = {0, 0, 0};
        return black;
    }

    if (y < 0 || y >= buffer->height) {
        Color black = {0, 0, 0};
        return black;
    }

    int index = y * buffer->width + x;
    return buffer->data[index];
}