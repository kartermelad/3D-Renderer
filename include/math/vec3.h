#ifndef VEC3_H
#define VEC3_H

// A 3D vector with x, y, and z components
typedef struct {
    float x, y, z;
} Vec3;

/**
 * Add two vectors component-wise
 * 
 * @param a First vector
 * @param b Second vector
 * @return The addition of the two vectors
 */
Vec3 vec3_add(Vec3 a, Vec3 b);

/**
 * Subtract two vectors component-wise
 * 
 * @param a First vector
 * @param b Second vector
 * @return The subtraction of the two vectors
 */
Vec3 vec3_sub(Vec3 a, Vec3 b);

/**
 * Compute the dot product of two vectors
 * 
 * @param a First vector
 * @param b Second vector
 * @return The dot product of the two vectors
 */
float vec3_dot(Vec3 a, Vec3 b);

/**
 * Compute the cross product of two vectors
 * 
 * @param a First vector
 * @param b Second vector
 * @return The cross product of the two vectors
 */
Vec3 vec3_cross(Vec3 a, Vec3 b);

/**
 * Normalize a vector to have a magnitude of 1
 * 
 * @param v The vector to normalize
 * @return The normalized vector
 */
Vec3 vec3_normalize(Vec3 v);

/**
 * Compute the length of a vector
 * 
 * @param v The vector whose length will be computed
 * @return the length of the vector
 */
float vec3_length(Vec3 v);

/**
 * Scale a vector by a scalar value
 * 
 * @param v The vector to scale
 * @param scalar The scalar value
 * @return The scaled vector
 */
Vec3 vec3_scale(Vec3 v, float scalar);


/**
 * Rotates a vector around a given axis by a specified angle
 * 
 * @param v The vector to rotate
 * @param axis The axis to rotate arounds
 * @param angle_radians The angle to rotate, in radians
 * @return The rotated vector
 */
Vec3 vec3_rotate(Vec3 v, Vec3 axis, float angle_radians);

#endif