#ifndef PIXEL_BUFFER_H
#define PIXEL_BUFFER_H
#include <stdint.h>

// Represents a color with red, green, blue components
typedef struct {
    uint8_t r, g, b;
} Color;

// Represents a 2D pixel buffer for rendering
typedef struct {
    int width;
    int height;
    Color* data;
} PixelBuffer;

/**
 * Allocates memory and initializes the pixel buffer
 * 
 * @param width Width of the pixel buffer
 * @param height Height of the pixel buffer
 * @return A pointer to the newly created PixelBuffer
 */
PixelBuffer* create_pixel_buffer(int width, int height);

/**
 * Frees the memory to avoid leaks
 * 
 * @param buffer Pointer to the PixelBuffer to destroy
 * @return True if the buffer was successfully destroyed
 */
void destroy_pixel_buffer(PixelBuffer* buffer);

/**
 * Updates the color of a single pixel
 * 
 * @param buffer Pointer to the PixelBuffer to modify
 * @param x x-coordinate of the pixel to update
 * @param y y-coordinate of the pixel to update
 * @param color New color to set for the pixel
 */
void set_pixel(PixelBuffer* buffer, int x, int y, Color color);

/**
 * Reads the color of a specific pixel
 * 
 * @param buffer Pointer to the pixel to read from
 * @param x x-coordinate of the pixel to update
 * @param y y-coordinate of the pixel to update
 * @return The color of the pixel at the specified coordinates
 */
Color get_pixel(PixelBuffer* buffer, int x, int y);

/**
 * Fills the entire screen with a single color
 * 
 * @param buffer Pointer to the PixelBuffer to modify
 * @param clear_color The color to fill the entire buffer with
 */
void clear_buffer(PixelBuffer* buffer, Color clear_color);

#endif