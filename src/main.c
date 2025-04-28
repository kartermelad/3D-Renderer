#include <stdio.h>
#include "core/pixel_buffer.h"
#include "render/depth_buffer.h"
#include "render/renderer.h"
#include "math/mat4.h"
#include "math/vec3.h"

int main(void) {
    int width = 200, height = 200; // Increased resolution
    PixelBuffer* pixel_buffer = create_pixel_buffer(width, height);
    float* depth_buffer = create_depth_buffer(width, height);

    Color clear_color = {0, 0, 0, 255}; // Black background
    clear_buffer(pixel_buffer, clear_color);
    clear_depth_buffer(depth_buffer, width, height);

    Vertex v0 = {{-0.5f, -0.5f, 0.5f}};
    Vertex v1 = {{ 0.5f, -0.5f, 0.5f}};
    Vertex v2 = {{ 0.0f,  0.5f, 0.5f}};

    Mat4 mvp = mat4_identity();

    printf("Rendering triangle...\n");
    draw_triangle(v0, v1, v2, mvp, pixel_buffer, depth_buffer, width, height);
    printf("Triangle rendered. Saving to PPM...\n");
    save_to_ppm(pixel_buffer, "output.ppm");

    destroy_pixel_buffer(pixel_buffer);
    destroy_depth_buffer(depth_buffer);

    printf("Rendered triangle saved to output.ppm\n");
    return 0;
}
