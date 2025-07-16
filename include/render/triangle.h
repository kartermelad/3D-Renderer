#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "math/mat4.h"
#include "render/vertex.h"
#include "core/pixel_buffer.h"
#include "mesh/mesh.h"

/**
 * Draw a triangle on the screen using the given vertices and transformation matrix
 * 
 * @param v0 First vertex of the triangle
 * @param v1 Second vertex of the triangle
 * @param v2 Third vertex of the triangle
 * @param mvp Model-View-Projection matrix to transform the vertices
 * @param buffer Pixel buffer to draw the triangle onto
 * @param depth_buffer Depth buffer to handle depth testing
 * @param width Width of the pixel buffer
 * @param height Height of the pixel buffer
 */
void draw_triangle(Vertex v0, Vertex v1, Vertex v2, Mat4 mvp, PixelBuffer* buffer, float* depth_buffer, int width, int height);

/**
 * Draws the wireframe of a mesh using the given MVP matrix
 * 
 * @param mesh Pointer to the mesh to render as a wireframe
 * @param mvp Model-View-Projection matrix to transform the vertices
 * @param buffer Pixel buffer to draw the wireframe onto
 * @param depth_buffer Depth buffer to handle depth testing
 * @param width Width of the pixel buffer
 * @param height Height of the pixel buffer
 */
void draw_wireframe(const Mesh* mesh, Mat4 mvp, PixelBuffer* buffer, float* depth_buffer, int width, int height);

#endif