#include "core/camera.h"

// Calculates the forward direction of the camera
static Vec3 camera_forward(const Camera* camera) {
    Vec3 forward = vec3_sub(camera->target, camera->position);
    return vec3_normalize(forward);
}

// Calculates the right direction of the camera
static Vec3 camera_right(const Camera* camera) {
    Vec3 forward = camera_forward(camera);
    Vec3 right = vec3_cross(forward, camera->up);
    return vec3_normalize(right);
}

// Calculates the true up direction of the camera
static Vec3 camera_true_up(const Camera* camera) {
    Vec3 forward = camera_forward(camera);
    Vec3 right = camera_right(camera);
    Vec3 up = vec3_cross(right, forward);
    return vec3_normalize(up);
}

Mat4 camera_get_view_matrix(const Camera* camera) {
    Vec3 forward = camera_forward(camera);
    Vec3 right = camera_right(camera);
    Vec3 up = camera_true_up(camera);

    Mat4 matrix = mat4_identity();
    matrix.m[0] = right.x;
    matrix.m[1] = right.y;
    matrix.m[2] = right.z;
    matrix.m[3] = -vec3_dot(right, camera->position);
    matrix.m[4] = up.x;
    matrix.m[5] = up.y;
    matrix.m[6] = up.z;
    matrix.m[7] = -vec3_dot(up, camera->position);
    matrix.m[8] = -forward.x;
    matrix.m[9] = -forward.y;
    matrix.m[10] = -forward.z;
    matrix.m[11] = vec3_dot(forward, camera->position);
    matrix.m[12] = 0.0f;
    matrix.m[13] = 0.0f;
    matrix.m[14] = 0.0f;
    matrix.m[15] = 1.0f;

    return matrix;
}

void camera_move_forward(Camera* camera, float delta_time, float speed) {
    Vec3 forward = camera_forward(camera);
    float scalar = delta_time * speed;
    Vec3 vector = vec3_scale(forward, scalar);

    camera->position = vec3_add(camera->position, vector);
    camera->target = vec3_add(camera->position, forward);
}

void camera_move_backward(Camera* camera, float delta_time, float speed) {
    Vec3 forward = camera_forward(camera);
    float scalar = -delta_time * speed;
    Vec3 vector = vec3_scale(forward, scalar);

    camera->position = vec3_add(camera->position, vector);
    camera->target = vec3_add(camera->position, forward);
}

void camera_strafe_left(Camera* camera, float delta_time, float speed) {
    Vec3 right = camera_right(camera);
    float scalar = -delta_time * speed;
    Vec3 vector = vec3_scale(right, scalar);
    camera->position = vec3_add(camera->position, vector);
    camera->target = vec3_add(camera->target, vector);
}

void camera_strafe_right(Camera* camera, float delta_time, float speed) {
    Vec3 right = camera_right(camera);
    float scalar = delta_time * speed;
    Vec3 vector = vec3_scale(right, scalar);
    camera->position = vec3_add(camera->position, vector);
    camera->target = vec3_add(camera->target, vector);
}

void camera_yaw(Camera* camera, float angle_radians) {
    Vec3 forward = camera_forward(camera);
    Vec3 rotated = vec3_rotate(forward, camera->up, angle_radians);
    rotated = vec3_normalize(rotated);
    camera->target = vec3_add(camera->position, rotated);
}

void camera_pitch(Camera* camera, float angle_radians) {
    Vec3 forward = camera_forward(camera);
    Vec3 right = camera_right(camera);
    Vec3 rotated = vec3_rotate(forward, right, angle_radians);
    rotated = vec3_normalize(rotated);
    camera->target = vec3_add(camera->position, rotated);
    camera->up = vec3_cross(right, rotated);
    camera->up = vec3_normalize(camera->up);
}