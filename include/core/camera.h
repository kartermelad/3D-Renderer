#ifndef CAMERA_H
#define CAMERA_H

#include "math/vec3.h"
#include "math/mat4.h"

// A camera in 3D space
typedef struct {
    Vec3 position;
    Vec3 target;
    Vec3 up; 
    float yaw;
    float pitch;
} Camera;

/**
 * Computes the view matrix for the given camera
 * 
 * @param camera Pointer to the Camera structure
 * @return The view matrix as a Mat4
 */
Mat4 camera_get_view_matrix(const Camera* camera);

/**
 * Moves the camera forward in the direction it is facing
 * 
 * @param camera Pointer to the Camera structure
 * @param delta_time The time elapsed since the last frame
 * @param speed The speed at which the camera moves
 */
void camera_move_forward(Camera* camera, float delta_time, float speed);

/**
 * Moves the camera backward opposite to the direction it is facing
 * 
 * @param camera Pointer to the Camera structure
 * @param delta_time The time elapsed since the last frame
 * @param speed The speed at which the camera moves
 */
void camera_move_backward(Camera* camera, float delta_time, float speed);

/**
 * Moves the camera to the left
 * 
 * @param camera Pointer to the Camera structure
 * @param delta_time The time elapsed since the last frame
 * @param speed The speed at which the camera moves
 */
void camera_strafe_left(Camera* camera, float delta_time, float speed);

/**
 * Moves the camera to the right
 * 
 * @param camera Pointer to the Camera structure
 * @param delta_time The time elapsed since the last frame
 * @param speed The speed at which the camera moves
 */
void camera_strafe_right(Camera* camera, float delta_time, float speed);

/**
 * Rotates the camera around the up axis
 * 
 * @param camera Pointer to the Camera structure
 * @param angle_radians The angle to rotate in radians
 */
void camera_yaw(Camera* camera, float angle_radians);

/**
 * Rotates the camera around the right axis
 * 
 * @param camera Pointer to the Camera structure
 * @param angle_radians The angle to rotate in radians
 */
void camera_pitch(Camera* camera, float angle_radians);

#endif