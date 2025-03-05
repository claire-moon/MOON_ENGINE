#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

typedef struct {
	vec3 position;
	vec3 front;
	vec3 up;
	float yaw;
	float pitch;
	float speed;
	float sensitivity;
} Camera;

void Camera_Init(Camera* camera, vec3 position, vec3 front, vec3 up);
void Camera_Update(Camera* camera, float deltaTime);

#endif
