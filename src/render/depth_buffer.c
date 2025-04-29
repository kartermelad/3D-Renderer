#include "render/depth_buffer.h"
#include <stdlib.h>
#include <math.h>

float* create_depth_buffer(int width, int height) {
    int n = width * height;
    float* buffer = (float*)malloc(n * sizeof(float));
    if (!buffer) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        buffer[i] = INFINITY;
    }

    return buffer;
}

void clear_depth_buffer(float* buffer, int width, int height) {
    int n = width * height;

    for (int i = 0; i < n; i++) {
        buffer[i] = INFINITY;
    }
}

void destroy_depth_buffer(float* buffer) {
    if (!buffer) {
        return;
    }
    free(buffer);
}

float get_depth(float* depth_buffer, int width, int x, int y) {
    return depth_buffer[y * width + x];
}

void set_depth(float* depth_buffer, int width, int x, int y, float z) {
    depth_buffer[y * width + x] = z;
}