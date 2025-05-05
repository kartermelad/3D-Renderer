#ifndef MAT4_H
#define MAT4_H
#include "math/vec4.h"

// 4x4 matrix using a 1D array of 16 floats
typedef struct {
    float m[16];
} Mat4;

/**
 * Create an identity matrix
 * 
 * @return 4x4 identity matrix
 */
Mat4 mat4_identity();

/**
 * Multiply two 4x4 matrices
 * 
 * @param a First matrix
 * @param b Second matrix
 * @return result of matrix multiplication
 */
Mat4 mat4_multiply(Mat4 a, Mat4 b);

/**
 * Create a translation matrix
 * 
 * @param x translation along the x-axis
 * @param y translation along the y-axis
 * @param z translation along the z-axis
 * @return 4x4 translation matrix
 */
Mat4 mat4_translation(float x, float y, float z);

/**
 * Create a scaling matrix
 * 
 * @param x Scaling factor along the x-axis
 * @param y Scaling factor along the y-axis
 * @param z Scaling factor along the z-axis
 * @return 4x4 scaling matrix
 */
Mat4 mat4_scale(float x, float y, float z);

/**
 * Create a perspective projection matrix
 * 
 * @param fov Field of view in degrees
 * @param aspect Aspect ratio (width / height)
 * @param near Near clipping plane
 * @param far Far clipping plane
 * @return 4x4 perspective projection matrix
 */
Mat4 mat4_perspective(float fov, float aspect, float near, float far);

/**
 * Multiply a 4x4 matrix by a 4D vector
 * 
 * @param mat The 4x4 matrix
 * @param vec The 4D vector
 * @return The resulting 4D vector after multiplication
 */
Vec4 mat4_mul_vec4(Mat4 mat, Vec4 vec);

/**
 * Create a look-at matrix
 * 
 * @param eye Position of the camera
 * @param target Point the camera is looking at
 * @param up Up vector of the camera
 * @return 4x4 look-at matrix
 */
Mat4 mat4_look_at(Vec3 eye, Vec3 target, Vec3 up);

#endif