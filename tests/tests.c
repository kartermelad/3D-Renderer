#include <string.h>
#include "harness/unity.h"
#include "../include/core/pixel_buffer.h"
#include "../include/math/vec3.h"

PixelBuffer* buffer;

void setUp(void) {
    buffer = create_pixel_buffer(10, 10);
}

void tearDown(void) {
    destroy_pixel_buffer(buffer);
}

void test_create_pixel_buffer(void) {
    TEST_ASSERT_NOT_NULL(buffer);
    TEST_ASSERT_EQUAL_INT(10, buffer->width);
    TEST_ASSERT_EQUAL_INT(10, buffer->height);
    TEST_ASSERT_NOT_NULL(buffer->data);
}

void test_clear_buffer(void) {
    Color red = {255, 0, 0};
    clear_buffer(buffer, red);

    for (int i = 0; i < buffer->width * buffer->height; i++) {
        TEST_ASSERT_EQUAL_UINT8(255, buffer->data[i].r);
        TEST_ASSERT_EQUAL_UINT8(0, buffer->data[i].g);
        TEST_ASSERT_EQUAL_UINT8(0, buffer->data[i].b);
    }
}

void test_set_pixel(void) {
    Color green = {0, 255, 0};
    set_pixel(buffer, 5, 5, green);

    int index = 5 * buffer->width + 5;
    TEST_ASSERT_EQUAL_UINT8(0, buffer->data[index].r);
    TEST_ASSERT_EQUAL_UINT8(255, buffer->data[index].g);
    TEST_ASSERT_EQUAL_UINT8(0, buffer->data[index].b);
}

void test_get_pixel(void) {
    Color blue = {0, 0, 255};
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
    return UNITY_END();
}