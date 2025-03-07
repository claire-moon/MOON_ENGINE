#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

typedef struct {
	vec3 position;
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 worldUp;
	float yaw;
	float pitch;
	float speed;
	float sensitivity;
	float fov;
	mat4 view;
} Camera;

void Camera_Init(Camera* camera, vec3 position, vec3 up, float yaw, float pitch);
void Camera_UpdateVectors(Camera* camera);
void Camera_ProcessKeyboard(Camera* camera, int direction, float deltaTime);
void Camera_ProcessMouseMovement(Camera* camera, float xoffset, float yoffset, int constrainPitch);
void Camera_ProcessMouseScroll(Camera* camera, float yoffset);

#endif
