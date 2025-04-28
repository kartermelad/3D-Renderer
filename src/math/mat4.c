#define _USE_MATH_DEFINES
#include "math/mat4.h"
#include <math.h>

Mat4 mat4_identity() {
    Mat4 matrix = {{0.0f}};
    matrix.m[0] = 1.0f;
    matrix.m[5] = 1.0f;
    matrix.m[10] = 1.0f;
    matrix.m[15] = 1.0f;

    return matrix;
}

Mat4 mat4_multiply(Mat4 a, Mat4 b) {
    Mat4 matrix = {{0.0f}};
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix.m[i*4+j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                matrix.m[i*4+j] += a.m[i*4+k] * b.m[k*4+j];
            }
        }
    }

    return matrix;
}

Mat4 mat4_translation(float x, float y, float z) {
    Mat4 matrix = mat4_identity();
    matrix.m[12] = x;
    matrix.m[13] = y;
    matrix.m[14] = z;

    return matrix;
}

Mat4 mat4_scale(float x, float y, float z) {
    Mat4 matrix = mat4_identity();
    matrix.m[0] = x;
    matrix.m[5] = y;
    matrix.m[10] = z;

    return matrix;
}

Mat4 mat4_perspective(float fov, float aspect, float near, float far) {
    float fov_radians = fov * (M_PI / 180);
    float f = 1 / tan(fov_radians / 2);

    Mat4 matrix = {{0.0f}};
    matrix.m[0] = f / aspect;
    matrix.m[5] = f;
    matrix.m[10] = far / (far - near);
    matrix.m[11] = 1.0f;
    matrix.m[14] = -(near * far) / (far - near);

    return matrix;
}