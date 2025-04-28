#ifndef DEPTH_BUFFER_H
#define DEPTH_BUFFER_H

/**
 * Allocate memory for a depth buffer and initialize all values to infinity
 * 
 * @param width The width of the depth buffer
 * @param height The height of the depth buffer
 * @return A pointer to the allocated depth buffer
 */
float* create_depth_buffer(int width, int height);

/**
 * Reset all values in the depth buffer to INFINITY before rendering a new frame
 * 
 * @param depth_buffer The depth buffer to clear
 * @param width The width of the depth buffer
 * @param height The height of the depth buffer
 */
void clear_depth_buffer(float* depth_buffer, int width, int height);

/**
 * Free the memory allocated for the depth buffer
 * 
 * @param depth_buffer The depth buffer to destroy
 */
void destroy_depth_buffer(float* depth_buffer);

#endif 