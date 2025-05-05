#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include "harness/unity.h"
#include "../include/core/pixel_buffer.h"
#include "../include/core/camera.h"
#include "../include/math/vec3.h"
#include "../include/math/mat4.h"
#include "../include/render/depth_buffer.h"

PixelBuffer* buffer;
Camera camera;

void setUp(void) {
    buffer = create_pixel_buffer(10, 10);

    camera.position = (Vec3){0.0f, 0.0f, 0.0f};
    camera.target = (Vec3){0.0f, 0.0f, 1.0f};
    camera.up = (Vec3){0.0f, 1.0f, 0.0f};
    camera.yaw = 0.0f;
    camera.pitch = 0.0f;
}

void tearDown(void) {
    destroy_pixel_buffer(buffer);
}

void test_create_pixel_buffer(void) {
    TEST_ASSERT_NOT_NULL(buffer);
    TEST_ASSERT_EQUAL_INT(10, buffer->width);
    TEST_ASSERT_EQUAL_INT(10, buffer->height);
    TEST_ASSERT_NOT_NULL(buffer->pixels);
}

void test_clear_buffer(void) {
    Color red = {255, 0, 0, 255};
    clear_buffer(buffer, red);

    for (int i = 0; i < buffer->width * buffer->height; i++) {
        TEST_ASSERT_EQUAL_UINT8(255, buffer->pixels[i].r);
        TEST_ASSERT_EQUAL_UINT8(0, buffer->pixels[i].g);
        TEST_ASSERT_EQUAL_UINT8(0, buffer->pixels[i].b);
    }
}

void test_set_pixel(void) {
    Color green = {0, 255, 0, 255};
    set_pixel(buffer, 5, 5, green);

    int index = 5 * buffer->width + 5;
    TEST_ASSERT_EQUAL_UINT8(0, buffer->pixels[index].r);
    TEST_ASSERT_EQUAL_UINT8(255, buffer->pixels[index].g);
    TEST_ASSERT_EQUAL_UINT8(0, buffer->pixels[index].b);
}

void test_get_pixel(void) {
    Color blue = {0, 0, 255, 255};
    set_pixel(buffer, 2, 3, blue);

    Color result = get_pixel(buffer, 2, 3);
    TEST_ASSERT_EQUAL_UINT8(0, result.r);
    TEST_ASSERT_EQUAL_UINT8(0, result.g);
    TEST_ASSERT_EQUAL_UINT8(255, result.b);
}

void test_get_pixel_out_of_bounds(void) {
    Color result = get_pixel(buffer, -1, -1);
    TEST_ASSERT_EQUAL_UINT8(0, result.r);
    TEST_ASSERT_EQUAL_UINT8(0, result.g);
    TEST_ASSERT_EQUAL_UINT8(0, result.b);

    result = get_pixel(buffer, 20, 20);
    TEST_ASSERT_EQUAL_UINT8(0, result.r);
    TEST_ASSERT_EQUAL_UINT8(0, result.g);
    TEST_ASSERT_EQUAL_UINT8(0, result.b);
}

void test_vec3_add(void) {
    Vec3 a = {1.0f, 2.0f, 3.0f};
    Vec3 b = {4.0f, 5.0f, 6.0f};
    Vec3 result = vec3_add(a, b);

    TEST_ASSERT_EQUAL_FLOAT(5.0f, result.x);
    TEST_ASSERT_EQUAL_FLOAT(7.0f, result.y);
    TEST_ASSERT_EQUAL_FLOAT(9.0f, result.z);
}

void test_vec3_sub(void) {
    Vec3 a = {4.0f, 5.0f, 6.0f};
    Vec3 b = {1.0f, 2.0f, 3.0f};
    Vec3 result = vec3_sub(a, b);

    TEST_ASSERT_EQUAL_FLOAT(3.0f, result.x);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, result.y);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, result.z);
}

void test_vec3_dot(void) {
    Vec3 a = {1.0f, 2.0f, 3.0f};
    Vec3 b = {4.0f, 5.0f, 6.0f};
    float result = vec3_dot(a, b);

    TEST_ASSERT_EQUAL_FLOAT(32.0f, result);
}

void test_vec3_cross(void) {
    Vec3 a = {1.0f, 0.0f, 0.0f};
    Vec3 b = {0.0f, 1.0f, 0.0f};
    Vec3 result = vec3_cross(a, b);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, result.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, result.y);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, result.z);
}

void test_vec3_normalize(void) {
    Vec3 v = {3.0f, 4.0f, 0.0f};
    Vec3 normalized = vec3_normalize(v);

    float length = vec3_length(normalized);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, length);
    TEST_ASSERT_EQUAL_FLOAT(0.6f, normalized.x);
    TEST_ASSERT_EQUAL_FLOAT(0.8f, normalized.y);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, normalized.z);
}

void test_vec3_length(void) {
    Vec3 v = {3.0f, 4.0f, 0.0f};
    float length = vec3_length(v);

    TEST_ASSERT_EQUAL_FLOAT(5.0f, length);
}

void test_vec3_scale(void) {
    Vec3 v = {1.0f, 2.0f, 3.0f};
    float scalar = 2.0f;
    Vec3 result = vec3_scale(v, scalar);

    TEST_ASSERT_EQUAL_FLOAT(2.0f, result.x);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, result.y);
    TEST_ASSERT_EQUAL_FLOAT(6.0f, result.z);
}

void test_mat4_identity(void) {
    Mat4 matrix = mat4_identity();

    TEST_ASSERT_EQUAL_FLOAT(1.0f, matrix.m[0]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, matrix.m[5]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, matrix.m[10]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, matrix.m[15]);

    for (int i = 0; i < 16; i++) {
        if (i != 0 && i != 5 && i != 10 && i != 15) {
            TEST_ASSERT_EQUAL_FLOAT(0.0f, matrix.m[i]);
        }
    }
}

void test_mat4_translation(void) {
    Mat4 matrix = mat4_translation(1.0f, 2.0f, 3.0f);

    TEST_ASSERT_EQUAL_FLOAT(1.0f, matrix.m[0]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, matrix.m[5]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, matrix.m[10]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, matrix.m[15]);

    TEST_ASSERT_EQUAL_FLOAT(1.0f, matrix.m[12]);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, matrix.m[13]);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, matrix.m[14]);

    for (int i = 0; i < 16; i++) {
        if (i != 0 && i != 5 && i != 10 && i != 15 && i != 12 && i != 13 && i != 14) {
            TEST_ASSERT_EQUAL_FLOAT(0.0f, matrix.m[i]);
        }
    }
}

void test_mat4_multiply(void) {
    Mat4 a = mat4_identity();
    Mat4 b = mat4_identity();
    Mat4 result = mat4_multiply(a, b);

    for (int i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL_FLOAT(a.m[i], result.m[i]);
    }
}

void test_mat4_scale(void) {
    Mat4 matrix = mat4_scale(2.0f, 3.0f, 4.0f);

    TEST_ASSERT_EQUAL_FLOAT(2.0f, matrix.m[0]);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, matrix.m[5]);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, matrix.m[10]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, matrix.m[15]);

    for (int i = 0; i < 16; i++) {
        if (i != 0 && i != 5 && i != 10 && i != 15) {
            TEST_ASSERT_EQUAL_FLOAT(0.0f, matrix.m[i]);
        }
    }
}

void test_mat4_perspective(void) {
    Mat4 matrix = mat4_perspective(90.0f, 1.0f, 0.1f, 100.0f);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, matrix.m[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, matrix.m[5]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.001001f, matrix.m[10]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -0.1001001f, matrix.m[14]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, matrix.m[11]);
}

void test_create_depth_buffer(void) {
    int width = 5;
    int height = 5;
    float* depth_buffer = create_depth_buffer(width, height);

    TEST_ASSERT_NOT_NULL(depth_buffer);

    int n = width * height;
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_FLOAT(INFINITY, depth_buffer[i]);
    }

    destroy_depth_buffer(depth_buffer);
}

void test_clear_depth_buffer(void) {
    int width = 5;
    int height = 5;
    float* depth_buffer = create_depth_buffer(width, height);

    int n = width * height;
    for (int i = 0; i < n; i++) {
        depth_buffer[i] = 0.0f;
    }

    clear_depth_buffer(depth_buffer, width, height);

    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_FLOAT(INFINITY, depth_buffer[i]);
    }

    destroy_depth_buffer(depth_buffer);
}

void test_destroy_depth_buffer(void) {
    int width = 5;
    int height = 5;
    float* depth_buffer = create_depth_buffer(width, height);

    TEST_ASSERT_NOT_NULL(depth_buffer);

    destroy_depth_buffer(depth_buffer);
    destroy_depth_buffer(NULL);\
}

void test_camera_move_forward(void) {
    camera_move_forward(&camera, 1.0f, 2.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.position.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.position.y);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, camera.position.z);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.target.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.target.y);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, camera.target.z);
}

void test_camera_move_backward(void) {
    camera_move_backward(&camera, 1.0f, 2.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.position.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.position.y);
    TEST_ASSERT_EQUAL_FLOAT(-2.0f, camera.position.z);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.target.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.target.y);
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, camera.target.z);
}

void test_camera_strafe_left(void) {
    camera_strafe_left(&camera, 1.0f, 2.0f);
    TEST_ASSERT_EQUAL_FLOAT(-2.0f, camera.position.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.position.y);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.position.z);

    TEST_ASSERT_EQUAL_FLOAT(-2.0f, camera.target.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.target.y);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, camera.target.z);
}

void test_camera_strafe_right(void) {
    camera_strafe_right(&camera, 1.0f, 2.0f);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, camera.position.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.position.y);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.position.z);

    TEST_ASSERT_EQUAL_FLOAT(2.0f, camera.target.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, camera.target.y);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, camera.target.z);
}

void test_camera_yaw(void) {
    Vec3 forward = vec3_sub(camera.target, camera.position);
    forward = vec3_normalize(forward);

    camera_yaw(&camera, M_PI / 2.0f);

    Vec3 expected_forward = { 1.0f, 0.0f, 0.0f }; // Forward = +X after 90° yaw
    Vec3 new_forward = vec3_sub(camera.target, camera.position);
    new_forward = vec3_normalize(new_forward);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected_forward.x, new_forward.x);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected_forward.y, new_forward.y);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected_forward.z, new_forward.z);
}

void test_camera_pitch(void) {
    Vec3 forward = vec3_sub(camera.target, camera.position);
    forward = vec3_normalize(forward);

    Vec3 right = vec3_cross(forward, camera.up);
    right = vec3_normalize(right);

    camera_pitch(&camera, M_PI / 4.0f);

    Vec3 expected_forward = { 0.0f, 0.7071068f, 0.7071068f };

    Vec3 new_forward = vec3_sub(camera.target, camera.position);
    new_forward = vec3_normalize(new_forward);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected_forward.x, new_forward.x);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected_forward.y, new_forward.y);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected_forward.z, new_forward.z);
}

void test_mat4_look_at(void) {
    Vec3 eye = {0.0f, 0.0f, 5.0f};
    Vec3 target = {0.0f, 0.0f, 0.0f};
    Vec3 up = {0.0f, 1.0f, 0.0f};

    Mat4 view_matrix = mat4_look_at(eye, target, up);

    Vec3 forward = vec3_normalize(vec3_sub(target, eye));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -forward.x, view_matrix.m[8]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -forward.y, view_matrix.m[9]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -forward.z, view_matrix.m[10]);

    Vec3 right = vec3_normalize(vec3_cross(up, forward));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, right.x, view_matrix.m[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, right.y, view_matrix.m[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, right.z, view_matrix.m[2]);

    Vec3 true_up = vec3_cross(forward, right);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, true_up.x, view_matrix.m[4]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, true_up.y, view_matrix.m[5]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, true_up.z, view_matrix.m[6]);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -vec3_dot(right, eye), view_matrix.m[12]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -vec3_dot(true_up, eye), view_matrix.m[13]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, vec3_dot(forward, eye), view_matrix.m[14]);

    TEST_ASSERT_EQUAL_FLOAT(0.0f, view_matrix.m[3]);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, view_matrix.m[7]);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, view_matrix.m[11]);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, view_matrix.m[15]);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_pixel_buffer);
    RUN_TEST(test_clear_buffer);
    RUN_TEST(test_set_pixel);
    RUN_TEST(test_get_pixel);
    RUN_TEST(test_get_pixel_out_of_bounds);
    RUN_TEST(test_vec3_add);
    RUN_TEST(test_vec3_sub);
    RUN_TEST(test_vec3_dot);
    RUN_TEST(test_vec3_cross);
    RUN_TEST(test_vec3_normalize);
    RUN_TEST(test_vec3_length);
    RUN_TEST(test_vec3_scale);
    RUN_TEST(test_mat4_identity);
    RUN_TEST(test_mat4_translation);
    RUN_TEST(test_mat4_multiply);
    RUN_TEST(test_mat4_scale);
    RUN_TEST(test_mat4_perspective);
    RUN_TEST(test_create_depth_buffer);
    RUN_TEST(test_clear_depth_buffer);
    RUN_TEST(test_destroy_depth_buffer);
    RUN_TEST(test_camera_move_backward);
    RUN_TEST(test_camera_move_forward);
    RUN_TEST(test_camera_strafe_left);
    RUN_TEST(test_camera_strafe_right);
    RUN_TEST(test_camera_yaw);
    RUN_TEST(test_camera_pitch);
    RUN_TEST(test_mat4_look_at);
    return UNITY_END();
}