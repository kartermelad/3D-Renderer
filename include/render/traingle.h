#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "render/vertex.h"
#include "core/pixel_buffer.h"

/**
 * Draws a wireframe triangle on the pixel buffer.
 * 
 * @param v0 Pointer to the first vertex of the triangle
 * @param v1 Pointer to the second vertex of the triangle
 * @param v2 Pointer to the third vertex of the triangle
 * @param buffer Pointer to the pixel buffer where the triangle will be drawn
 */
void draw_wireframe_triangle(const Vertex* v0, const Vertex* v1, const Vertex* v2, PixelBuffer* buffer);

/**
 * Draws a filled triangle on the pixel buffer with depth testing.
 * 
 * @param v0 Pointer to the first vertex of the triangle
 * @param v1 Pointer to the second vertex of the triangle
 * @param v2 Pointer to the third vertex of the triangle
 * @param buffer Pointer to the pixel buffer where the triangle will be drawn
 * @param depth_buffer Pointer to the depth buffer for depth testing
 */
void draw_filled_triangle(const Vertex* v0, const Vertex* v1, const Vertex* v2, PixelBuffer* buffer, float* depth_buffer);

#endif