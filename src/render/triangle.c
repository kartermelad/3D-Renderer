#include "render/traingle.h"
#include "math/vec2.h"
#include <stdlib.h>

// Draws a straight line between two 2D points
static void draw_line(Vec2 p0, Vec2 p1, Color color, PixelBuffer* buffer) {
    int x0 = (int)p0.x;
    int y0 = (int)p0.y;
    int x1 = (int)p1.x;
    int y1 = (int)p1.y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        set_pixel(buffer, x0, y0, color);

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// Fills a triangle where two points at the bottom have the same y value
static void fill_flat_bottom_triangle(const Vertex* v0, const Vertex* v1, const Vertex* v2, PixelBuffer* buffer, float* depth_buffer) {
    float inv_slope1 = (v1->position.x - v0->position.x) / (v1->position.y - v0->position.y);
    float inv_slope2 = (v2->position.x - v0->position.x) / (v2->position.y - v0->position.y);

    float x1 = v0->position.x;
    float x2 = v0->position.x;

    for (int y = (int)v0->position.y; y <= (int)v1->position.y; y++) {
        draw_line((Vec2){x1, y}, (Vec2){x2, y}, v0->color, buffer);
        x1 += inv_slope1;
        x2 += inv_slope2;
    }
}

// Fills a triangle where two points at the top have the same same y value
static void fill_flat_top_triangle(const Vertex* v0, const Vertex* v1, const Vertex* v2, PixelBuffer* buffer, float* depth_buffer) {
    float inv_slope1 = (v2->position.x - v0->position.x) / (v2->position.y - v0->position.y);
    float inv_slope2 = (v2->position.x - v1->position.x) / (v2->position.y - v1->position.y);

    float cur_x1 = v2->position.x;
    float cur_x2 = v2->position.x;

    for (int y = (int)v0->position.y; y <= (int)v2->position.y; y++)    {
        draw_line((Vec2){cur_x1, y}, (Vec2){cur_x2, y}, v0->color, buffer);
        cur_x1 += inv_slope1;
        cur_x2 += inv_slope2;        
    }
}

void draw_wireframe_triangle(const Vertex* v0, const Vertex* v1, const Vertex* v2, PixelBuffer* buffer) {
    Vec2 p0 = {v0->position.x, v0->position.y};
    Vec2 p1 = {v1->position.x, v1->position.y};
    Vec2 p2 = {v2->position.x, v2->position.y};

    Color wireframe_color = {255, 255, 0, 0};

    draw_line(p0, p1, wireframe_color, buffer);
    draw_line(p1, p2, wireframe_color, buffer);
    draw_line(p2, p0, wireframe_color, buffer);
}

void draw_filled_triangle(const Vertex* v0, const Vertex* v1, const Vertex* v2, PixelBuffer* buffer, float* depth_buffer) {
    if (v0->position.y > v1->position.y) {
        const Vertex* temp = v0;
        v0 = v1;
        v1 = temp;
    }
    if (v0->position.y > v2->position.y) {
        const Vertex* temp = v0;
        v0 = v2;
        v2 = temp;
    }
    if (v1->position.y > v2->position.y) {
        const Vertex* temp = v1;
        v1 = v2;
        v2 = temp;
    }

    if (v1->position.y == v2->position.y) {
        fill_flat_bottom_triangle(v0, v1, v2, buffer, depth_buffer);
    } else if (v0->position.y == v1->position.y) {
        fill_flat_top_triangle(v0, v1, v2, buffer, depth_buffer);
    } else {
        Vertex v3 = {
            .position = {
                v0->position.x + ((v1->position.y - v0->position.y) / (v2->position.y - v0->position.y)) * (v2->position.x - v0->position.x),
                v1->position.y,
                v0->position.z + ((v1->position.y - v0->position.y) / (v2->position.y - v0->position.y)) * (v2->position.z - v0->position.z)
            },
            .color = v0->color
        };

        fill_flat_bottom_triangle(v0, v1, &v3, buffer, depth_buffer);
        fill_flat_top_triangle(&v3, v1, v2, buffer, depth_buffer);
    }
}