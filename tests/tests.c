#include <string.h>
#include "harness/unity.h"
#include "../include/core/pixel_buffer.h"

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

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_pixel_buffer);
    RUN_TEST(test_clear_buffer);
    RUN_TEST(test_set_pixel);
    RUN_TEST(test_get_pixel);
    RUN_TEST(test_get_pixel_out_of_bounds);
    return UNITY_END();
}