#ifndef VEC2_H
#define VEC2_H

// 2D vector with x and y components
typedef struct {
    float x, y;
} Vec2;

/**
 * Compute the cross product of two 2D vectors
 * 
 * @param a First 2D vector
 * @param b Second 2D vector
 * @return Scalar cross product of the two vectors
 */
float vec2_cross(Vec2 a, Vec2 b);

/**
 * Compute the subtraction of two 2D vectors
 * 
 * @param a First 2D vector
 * @param b Second 2D vector
 * @return Scalar subtraction of the two vectors
 */
Vec2 vec2_subtract(Vec2 a, Vec2 b);

#endif