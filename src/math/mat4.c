#include "math/mat4.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Mat4 mat4_identity() {
    Mat4 matrix = {{0.0f}};
    matrix.m[0] = 1.0f;
    matrix.m[5] = 1.0f;
    matrix.m[10] = 1.0f;
    matrix.m[15] = 1.0f;

    return matrix;
}

Mat4 mat4_multiply(Mat4 a, Mat4 b) {
    Mat4 result = {{0.0f}};

    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += a.m[k * 4 + row] * b.m[col * 4 + k];
            }
            result.m[col * 4 + row] = sum;
        }
    }
    return result;
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
    matrix.m[14] = (near * far) / (far - near);

    return matrix;
}

Vec4 mat4_mul_vec4(Mat4 mat, Vec4 vec) {
    Vec4 result;
    result.x = mat.m[0] * vec.x + mat.m[4] * vec.y + mat.m[8] * vec.z + mat.m[12] * vec.w;
    result.y = mat.m[1] * vec.x + mat.m[5] * vec.y + mat.m[9] * vec.z + mat.m[13] * vec.w;
    result.z = mat.m[2] * vec.x + mat.m[6] * vec.y + mat.m[10] * vec.z + mat.m[14] * vec.w;
    result.w = mat.m[3] * vec.x + mat.m[7] * vec.y + mat.m[11] * vec.z + mat.m[15] * vec.w;
    return result;
}

Mat4 mat4_look_at(Vec3 eye, Vec3 target, Vec3 up) {
    Vec3 forward = vec3_normalize(vec3_sub(target, eye));
    Vec3 right = vec3_normalize(vec3_cross(up, forward));
    Vec3 true_up = vec3_cross(forward, right);

    Mat4 result = mat4_identity();

    result.m[0] = right.x;
    result.m[1] = right.y;
    result.m[2] = right.z;

    result.m[4] = true_up.x;
    result.m[5] = true_up.y;
    result.m[6] = true_up.z;

    result.m[8] = forward.x;
    result.m[9] = forward.y;
    result.m[10] = forward.z;

    result.m[12] = -vec3_dot(right, eye);
    result.m[13] = -vec3_dot(true_up, eye);
    result.m[14] = -vec3_dot(forward, eye);

    return result;
}

Mat4 mat4_rotation_y(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    Mat4 m = mat4_identity();
    m.m[0] = c;
    m.m[2] = s;
    m.m[8] = -s;
    m.m[10] = c;
    return m;
}