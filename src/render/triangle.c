#include "render/triangle.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// Convert normalized device coordinates (NDC) to screen space
static Vec3 ndc_to_screen(Vec3 ndc, int width, int height) {
    Vec3 screen;
    screen.x = (ndc.x + 1) * (width / 2);
    screen.y = (1 - ndc.y) * (height / 2);
    screen.z = ndc.z;

    return screen;
}

// Compute the edge function for a triangle
static float edge_function(Vec2 a, Vec2 b, Vec2 c) {
    float result = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return result;
}

// Check if a point is inside a triangle using barycentric coordinates
static bool is_inside_triangle(Vec3 barycentric_coords) {
    bool all_positive = (barycentric_coords.x >= 0) && (barycentric_coords.y >= 0) && (barycentric_coords.z >= 0);
    bool all_negative = (barycentric_coords.x <= 0) && (barycentric_coords.y <= 0) && (barycentric_coords.z <= 0);
    return all_positive || all_negative;
}


// Interpolate depth value using barycentric coordinates
static float interpolate_depth(Vec3 barycentric, float z0, float z1, float z2) {
    float result = (barycentric.x * z0) + (barycentric.y * z1) + (barycentric.z * z2);
    return result;
}

// Uses fmin to find the smallest value among three floating point numbers
static float min(float a, float b, float c) {
    return fmin(fmin(a, b), c);
}

// Uses fmax to find the largest value among three floating point numbers
static float max(float a, float b, float c) {
    return fmax(fmax(a, b), c);
}

void draw_triangle(Vertex v0, Vertex v1, Vertex v2, Mat4 mvp, PixelBuffer* buffer, float* depth_buffer, int width, int height) {
    Vec4 vec0 = vertex_to_vec4(v0);
    Vec4 vec1 = vertex_to_vec4(v1);
    Vec4 vec2 = vertex_to_vec4(v2);

    vec0 = mat4_mul_vec4(mvp, vec0);
    vec1 = mat4_mul_vec4(mvp, vec1);
    vec2 = mat4_mul_vec4(mvp, vec2);

    Vec3 ndc0 = {vec0.x / vec0.w, vec0.y / vec0.w, vec0.z / vec0.w};
    Vec3 ndc1 = {vec1.x / vec1.w, vec1.y / vec1.w, vec1.z / vec1.w};
    Vec3 ndc2 = {vec2.x / vec2.w, vec2.y / vec2.w, vec2.z / vec2.w};

    ndc0.z = (ndc0.z + 1.0f) * 0.5f;
    ndc1.z = (ndc1.z + 1.0f) * 0.5f;
    ndc2.z = (ndc2.z + 1.0f) * 0.5f;

    Vec3 screen0 = ndc_to_screen(ndc0, width, height);
    Vec3 screen1 = ndc_to_screen(ndc1, width, height);
    Vec3 screen2 = ndc_to_screen(ndc2, width, height);

    Vec2 p0 = {screen0.x, screen0.y};
    Vec2 p1 = {screen1.x, screen1.y};
    Vec2 p2 = {screen2.x, screen2.y};

    int min_x = fmax(0, floor(min(p0.x, p1.x, p2.x)));
    int max_x = fmin(width - 1, ceil(max(p0.x, p1.x, p2.x)));
    int min_y = fmax(0, floor(min(p0.y, p1.y, p2.y)));
    int max_y = fmin(height - 1, ceil(max(p0.y, p1.y, p2.y)));
    float total_area = edge_function(p0, p1, p2);
    
    if (total_area < 0) {
        Vec2 temp = p1;
        p1 = p2;
        p2 = temp;
    
        total_area = edge_function(p0, p1, p2);
    }
    
    if (total_area <= 0) {
        return;
    }

    float inv_area = 1.0f / total_area;

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            Vec2 pixel_center = {x + 0.5f, y + 0.5f};

            float w0 = edge_function(p1, p2, pixel_center);
            float w1 = edge_function(p2, p0, pixel_center);
            float w2 = edge_function(p0, p1, pixel_center);
            Vec3 barycentric_coords = {w0, w1, w2};

            if (is_inside_triangle(barycentric_coords)) {
                float alpha = w0 * inv_area;
                float beta = w1 * inv_area;
                float gamma = w2 * inv_area;

                float depth = interpolate_depth((Vec3){alpha, beta, gamma}, screen0.z, screen1.z, screen2.z);
                int index = y * width + x;

                if (depth < depth_buffer[index]) {
                    depth_buffer[index] = depth;

                    Color color = {
                        .r = fminf(fmaxf(alpha * v0.color.r + beta * v1.color.r + gamma * v2.color.r, 0.0f), 255.0f),
                        .g = fminf(fmaxf(alpha * v0.color.g + beta * v1.color.g + gamma * v2.color.g, 0.0f), 255.0f),
                        .b = fminf(fmaxf(alpha * v0.color.b + beta * v1.color.b + gamma * v2.color.b, 0.0f), 255.0f),
                        .a = fminf(fmaxf(alpha * v0.color.a + beta * v1.color.a + gamma * v2.color.a, 0.0f), 255.0f)
                    };

                    set_pixel(buffer, x, y, color);
                }
            }
        }
    }
}
