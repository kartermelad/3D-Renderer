#ifndef VERTEX_H
#define VERTEX_H
#include "math/vec3.h"
#include "math/vec4.h"
#include "core/pixel_buffer.h"

// Vertex with a 3D position
typedef struct {
    Vec3 position;
    Color color;
    Vec3 normal;
} Vertex;

/**
 * Convert a Vertex to a 4D vector
 * 
 * @param v The vertex to convert
 * @return A 4D vector with the vertex's position as the x, y, z, and w components
 */
Vec4 vertex_to_vec4(Vertex v);

#endif