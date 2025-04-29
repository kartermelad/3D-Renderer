#include <stdio.h>
#include <GLFW/glfw3.h>
#include "core/pixel_buffer.h"
#include "render/depth_buffer.h"
#include "render/traingle.h"

#define WIDTH 800
#define HEIGHT 600

void upload_pixel_buffer_to_texture(PixelBuffer* buffer, GLuint texture_id) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, buffer->width, buffer->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main(void) {
    // Step 1: Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D Renderer", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Step 2: Create Pixel Buffer and Depth Buffer
    PixelBuffer* pixel_buffer = create_pixel_buffer(WIDTH, HEIGHT);
    float* depth_buffer = create_depth_buffer(WIDTH, HEIGHT);

    // Step 3: Create OpenGL Texture
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Step 4: Define Triangle Vertices
    Vertex v0 = {{400.0f, 100.0f, 0.5f}, {255, 0, 0, 255}, {0.0f, 0.0f, 0.0f}};
    Vertex v1 = {{600.0f, 400.0f, 0.5f}, {0, 255, 0, 255}, {0.0f, 0.0f, 0.0f}};
    Vertex v2 = {{200.0f, 400.0f, 0.5f}, {0, 0, 255, 255}, {0.0f, 0.0f, 0.0f}};

    // Step 5: Main Loop
    while (!glfwWindowShouldClose(window)) {
        // Clear PixelBuffer and DepthBuffer
        clear_buffer(pixel_buffer, (Color){0, 0, 0, 255}); // Black
        clear_depth_buffer(depth_buffer, WIDTH, HEIGHT);

        // Draw the triangle
        draw_filled_triangle(&v0, &v1, &v2, pixel_buffer, depth_buffer);

        // Upload PixelBuffer to OpenGL texture
        upload_pixel_buffer_to_texture(pixel_buffer, texture_id);

        // Render the texture to fill the window
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Step 6: Clean Up
    destroy_pixel_buffer(pixel_buffer);
    destroy_depth_buffer(depth_buffer);
    glDeleteTextures(1, &texture_id);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}