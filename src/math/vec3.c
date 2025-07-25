#include "math/vec3.h"
#include "math/vec4.h"
#include <math.h>

Vec3 vec3_add(Vec3 a, Vec3 b) {
    float x = a.x + b.x;
    float y = a.y + b.y;
    float z = a.z + b.z;

    Vec3 result = {x, y, z};
    return result;
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    float x = a.x - b.x;
    float y = a.y - b.y;
    float z = a.z - b.z;

    Vec3 result = {x, y, z};
    return result;
}

float vec3_dot(Vec3 a, Vec3 b) {
    float x = a.x * b.x;
    float y = a.y * b.y;
    float z = a.z * b.z;

    float result = x + y + z;
    return result;
}

Vec3 vec3_cross(Vec3 a, Vec3 b) {
    float x = (a.y * b.z) - (a.z * b.y);
    float y = (a.z * b.x) - (a.x * b.z);
    float z = (a.x * b.y) - (a.y * b.x);

    Vec3 result = {x, y, z};
    return result;
}

Vec3 vec3_normalize(Vec3 v) {
    float length = vec3_length(v);
    if (length == 0) return (Vec3){0.0f, 0.0f, 0.0f};

    float x = v.x / length;
    float y = v.y / length;
    float z = v.z / length;

    Vec3 result = {x, y, z};
    return result;
}

float vec3_length(Vec3 v) {
    float length = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    return length;
}

Vec3 vec3_scale(Vec3 v, float scalar) {
    float x = v.x * scalar;
    float y = v.y * scalar;
    float z = v.z * scalar;

    Vec3 result = {x, y, z};
    return result;
}

Vec3 vec3_rotate(Vec3 v, Vec3 axis, float angle_radians) {
    axis = vec3_normalize(axis);
    float cos_theta = cos(angle_radians);
    float sin_theta = sin(angle_radians);

    Vec3 term1 = vec3_scale(v, cos_theta);
    Vec3 term2 = vec3_scale(vec3_cross(v, axis), sin_theta);
    Vec3 term3 = vec3_scale(axis, vec3_dot(axis, v) * (1.0f - cos_theta));

    return vec3_add(vec3_add(term1, term2), term3);
}

Vec3 vec4_to_vec3(Vec4 v) {
    Vec3 result = { v.x, v.y, v.z };
    return result;
}

Vec3 compute_triangle_normal(Vec4 a, Vec4 b, Vec4 c) {
    Vec3 ab = vec3_sub(vec4_to_vec3(b), vec4_to_vec3(a));
    Vec3 ac = vec3_sub(vec4_to_vec3(c), vec4_to_vec3(a));
    return vec3_normalize(vec3_cross(ab, ac));
}