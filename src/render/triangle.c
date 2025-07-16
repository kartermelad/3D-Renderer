#include "render/triangle.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "mesh/mesh.h"
#include <stdint.h>
#include <stdlib.h>

static Vec3 ndc_to_screen(Vec3 ndc, int width, int height) {
    Vec3 screen;
    screen.x = (ndc.x + 1) * (width / 2);
    screen.y = (1 - ndc.y) * (height / 2);
    screen.z = ndc.z;
    return screen;
}

static float edge_function(Vec2 a, Vec2 b, Vec2 c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static bool is_inside_triangle(Vec3 barycentric_coords) {
    bool all_positive = (barycentric_coords.x >= 0) && (barycentric_coords.y >= 0) && (barycentric_coords.z >= 0);
    bool all_negative = (barycentric_coords.x <= 0) && (barycentric_coords.y <= 0) && (barycentric_coords.z <= 0);
    return all_positive || all_negative;
}

static float interpolate_depth(Vec3 barycentric, float z0, float z1, float z2) {
    return barycentric.x * z0 + barycentric.y * z1 + barycentric.z * z2;
}

static float min(float a, float b, float c) {
    return fmin(fmin(a, b), c);
}

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

    const float EDGE_THRESHOLD = 0.02f;
    const Color EDGE_COLOR  = {255, 255, 255, 255};

    Vec3 v0_world = {v0.position.x, v0.position.y, v0.position.z};
    Vec3 v1_world = {v1.position.x, v1.position.y, v1.position.z};
    Vec3 v2_world = {v2.position.x, v2.position.y, v2.position.z};

    Vec3 edge1 = vec3_sub(v1_world, v0_world);
    Vec3 edge2 = vec3_sub(v2_world, v0_world);
    Vec3 normal = vec3_normalize(vec3_cross(edge1, edge2));

    Vec3 view_dir = {0.0f, 0.0f, -1.0f};
    if (vec3_dot(normal, view_dir) > 0.0f) {
        return;
    }

    Vec3 light_dir = vec3_normalize((Vec3){1.0f, 2.0f, -2.0f});
    float intensity = fmaxf(0.2f, vec3_dot(normal, light_dir));

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            Vec2 pixel_center = {x + 0.5f, y + 0.5f};

            float w0 = edge_function(p1, p2, pixel_center);
            float w1 = edge_function(p2, p0, pixel_center);
            float w2 = edge_function(p0, p1, pixel_center);
            Vec3 barycentric_coords = {w0, w1, w2};

            if (is_inside_triangle(barycentric_coords)) {
                float alpha = w0 * inv_area;
                float beta  = w1 * inv_area;
                float gamma = w2 * inv_area;

                float depth = interpolate_depth((Vec3){alpha, beta, gamma}, screen0.z, screen1.z, screen2.z);
                int index = y * width + x;

                if (depth < depth_buffer[index]) {
                    depth_buffer[index] = depth;

                    if (alpha < EDGE_THRESHOLD || beta < EDGE_THRESHOLD || gamma < EDGE_THRESHOLD) {
                        set_pixel(buffer, x, y, EDGE_COLOR);
                    } else {
                        uint8_t base_r = (v0.color.r + v1.color.r + v2.color.r) / 3;
                        uint8_t base_g = (v0.color.g + v1.color.g + v2.color.g) / 3;
                        uint8_t base_b = (v0.color.b + v1.color.b + v2.color.b) / 3;

                        Color fill_color = {
                            .r = (uint8_t)fminf(255.0f, base_r * intensity),
                            .g = (uint8_t)fminf(255.0f, base_g * intensity),
                            .b = (uint8_t)fminf(255.0f, base_b * intensity),
                            .a = 255
                        };
                        set_pixel(buffer, x, y, fill_color);
                    }
                }
            }
        }
    }
}

// static uint64_t pack_edge(uint32_t a, uint32_t b) {
//     return ((uint64_t)a << 32) | b;
// }

static void draw_line(PixelBuffer* buf, int x0, int y0, int x1, int y1, Color c) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (true) {
        set_pixel(buf, x0, y0, c);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_wireframe(const Mesh* mesh, Mat4 mvp, PixelBuffer* buffer, float* depth_buffer, int width, int height) {
    (void)depth_buffer;
    Vec3* screen = malloc(sizeof *screen * mesh->vertexCount);
    for (size_t i = 0; i < mesh->vertexCount; i++) {
        Vec4 v = mat4_mul_vec4(mvp, vertex_to_vec4(mesh->vertices[i]));
        Vec3 ndc = {v.x/v.w, v.y/v.w, (v.z/v.w * 0.5f) + 0.5f};
        screen[i] = ndc_to_screen(ndc, width, height);
    }

    Edge* edges;
    size_t edgeCount = mesh_get_boundary_edges(mesh, &edges);

    const Color EDGE_COLOR = {255, 255, 255, 255};
    for (size_t e = 0; e < edgeCount; e++) {
        Vec3 A = screen[edges[e].a];
        Vec3 B = screen[edges[e].b];
        draw_line(buffer,
                  (int)roundf(A.x), (int)roundf(A.y),
                  (int)roundf(B.x), (int)roundf(B.y),
                  EDGE_COLOR);
    }

    free(edges);
    free(screen);
}
