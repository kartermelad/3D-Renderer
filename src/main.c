#include <stdio.h>
#include <GLFW/glfw3.h>
#include "core/pixel_buffer.h"
#include "core/camera.h"
#include "render/depth_buffer.h"
#include "render/triangle.h"
#include "math/mat4.h"
#include "mesh/mesh.h"

#define WIDTH 800
#define HEIGHT 600

void upload_pixel_buffer_to_texture(PixelBuffer* buffer, GLuint texture_id) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, buffer->width, buffer->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main(void) {
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

    PixelBuffer* pixel_buffer = create_pixel_buffer(WIDTH, HEIGHT);
    float* depth_buffer = create_depth_buffer(WIDTH, HEIGHT);

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    Camera camera;
    camera_init(&camera,
        (Vec3){0.0f, 0.0f, -10.0f},
        (Vec3){0.0f, 0.0f,  0.0f},
        (Vec3){0.0f, 1.0f,  0.0f},
        45.0f,
        (float)WIDTH / HEIGHT,
        0.1f,
        100.0f
    );

    Mesh* cube    = create_cube_mesh();
    Mesh* pyramid = create_pyramid_mesh();
    if (!cube || !pyramid) {
        fprintf(stderr, "Failed to create meshes\n");
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        clear_buffer(pixel_buffer, (Color){0,0,0,255});
        clear_depth_buffer(depth_buffer, WIDTH, HEIGHT);

        camera.view_matrix = camera_get_view_matrix(&camera);

        float angle = (float)glfwGetTime();

        Mat4 cube_model_matrix    = mat4_multiply(mat4_translation(-0.5f,  0.0f, 0.0f), mat4_rotation_y(angle));
        Mat4 pyramid_model_matrix = mat4_multiply(mat4_translation( 0.5f, -0.5f, 0.0f), mat4_rotation_y(angle));

        // Fill pass: draw triangles
        Mat4 cube_mvp    = mat4_multiply(camera.projection_matrix, mat4_multiply(camera.view_matrix, cube_model_matrix));
        for (size_t i = 0; i < cube->indexCount; i += 3) {
            draw_triangle(
                cube->vertices[cube->indices[i + 0]],
                cube->vertices[cube->indices[i + 1]],
                cube->vertices[cube->indices[i + 2]],
                cube_mvp,
                pixel_buffer,
                depth_buffer,
                WIDTH,
                HEIGHT
            );
        }

        Mat4 pyramid_mvp = mat4_multiply(camera.projection_matrix, mat4_multiply(camera.view_matrix, pyramid_model_matrix));
        for (size_t i = 0; i < pyramid->indexCount; i += 3) {
            draw_triangle(
                pyramid->vertices[pyramid->indices[i + 0]],
                pyramid->vertices[pyramid->indices[i + 1]],
                pyramid->vertices[pyramid->indices[i + 2]],
                pyramid_mvp,
                pixel_buffer,
                depth_buffer,
                WIDTH,
                HEIGHT
            );
        }

        // Wireframe pass: draw only boundary edges
        draw_wireframe(cube,    cube_mvp,    pixel_buffer, depth_buffer, WIDTH, HEIGHT);
        draw_wireframe(pyramid, pyramid_mvp, pixel_buffer, depth_buffer, WIDTH, HEIGHT);

        // Upload and display
        upload_pixel_buffer_to_texture(pixel_buffer, texture_id);

        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroy_mesh(cube);
    destroy_mesh(pyramid);
    destroy_pixel_buffer(pixel_buffer);
    destroy_depth_buffer(depth_buffer);
    glDeleteTextures(1, &texture_id);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}