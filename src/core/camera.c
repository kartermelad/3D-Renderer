#include "core/camera.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define CAMERA_PITCH_LIMIT (M_PI / 2 - 0.01f)

static void camera_update_target(Camera* camera) {
    float x = sinf(camera->yaw) * cosf(camera->pitch);
    float y = sinf(camera->pitch);
    float z = cosf(camera->yaw) * cosf(camera->pitch);

    Vec3 direction = { x, y, z };
    direction = vec3_normalize(direction);
    camera->target = vec3_add(camera->position, direction);
}

static Vec3 camera_forward(const Camera* camera) {
    return vec3_normalize(vec3_sub(camera->target, camera->position));
}

static Vec3 camera_right(const Camera* camera) {
    return vec3_normalize(vec3_cross(camera->up, camera_forward(camera)));
}

static Vec3 camera_true_up(const Camera* camera) {
    return vec3_normalize(vec3_cross(camera_forward(camera), camera_right(camera)));
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

    matrix.m[8] = forward.x;
    matrix.m[9] = forward.y;
    matrix.m[10] = forward.z;
    matrix.m[11] = -vec3_dot(forward, camera->position);

    matrix.m[12] = 0.0f;
    matrix.m[13] = 0.0f;
    matrix.m[14] = 0.0f;
    matrix.m[15] = 1.0f;

    return matrix;
}

void camera_move_forward(Camera* camera, float delta_time, float speed) {
    Vec3 delta = vec3_scale(camera_forward(camera), delta_time * speed);
    camera->position = vec3_add(camera->position, delta);
    camera->target = vec3_add(camera->target, delta);
}

void camera_move_backward(Camera* camera, float delta_time, float speed) {
    Vec3 delta = vec3_scale(camera_forward(camera), -delta_time * speed);
    camera->position = vec3_add(camera->position, delta);
    camera->target = vec3_add(camera->target, delta);
}

void camera_strafe_left(Camera* camera, float delta_time, float speed) {
    Vec3 delta = vec3_scale(camera_right(camera), -delta_time * speed);
    camera->position = vec3_add(camera->position, delta);
    camera->target = vec3_add(camera->target, delta);
}

void camera_strafe_right(Camera* camera, float delta_time, float speed) {
    Vec3 delta = vec3_scale(camera_right(camera), delta_time * speed);
    camera->position = vec3_add(camera->position, delta);
    camera->target = vec3_add(camera->target, delta);
}

void camera_yaw(Camera* camera, float angle_radians) {
    camera->yaw += angle_radians;
    camera_update_target(camera);
}

void camera_pitch(Camera* camera, float angle_radians) {
    camera->pitch += angle_radians;

    if (camera->pitch > CAMERA_PITCH_LIMIT) camera->pitch = CAMERA_PITCH_LIMIT;
    if (camera->pitch < -CAMERA_PITCH_LIMIT) camera->pitch = -CAMERA_PITCH_LIMIT;

    camera_update_target(camera);
}

void camera_init(Camera* camera, Vec3 position, Vec3 target, Vec3 up, float fov, float aspect, float near, float far) {
    camera->position = position;
    camera->target = target;
    camera->up = up;

    Vec3 forward = vec3_normalize(vec3_sub(target, position));
    camera->yaw = atan2f(forward.x, forward.z);
    camera->pitch = asinf(forward.y);

    if (camera->pitch > CAMERA_PITCH_LIMIT) camera->pitch = CAMERA_PITCH_LIMIT;
    if (camera->pitch < -CAMERA_PITCH_LIMIT) camera->pitch = -CAMERA_PITCH_LIMIT;

    camera->projection_matrix = mat4_perspective(fov, aspect, near, far);
    camera->view_matrix = camera_get_view_matrix(camera);
}