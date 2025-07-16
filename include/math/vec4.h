#ifndef VEC4_H
#define VEC4_H
#include "math/vec3.h"

// 4D vector with x, y, z, and w components
typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

/**
 * Create a 4D vector from a 3D vector and a w component
 * 
 * @param v 3D vector to use for the x, y, and z components
 * @param w Value to assign to the w component
 * @return A 4D vector with the specified components
 */
Vec4 vec4_from_vec3(Vec3 v, float w);

#endif