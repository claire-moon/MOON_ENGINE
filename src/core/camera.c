#include "camera.h"
#include <cglm/cglm.h>

void Camera_Init(Camera* camera, vec3 position, vec3 front, vec3 up) {
	glm_vec3_copy(position, camera->position);
	glm_vec3_copy(front, camera->front);
	glm_vec3_copy(up, camera->up);
	camera->yaw = -90.0f;
	camera->pitch = 0.0f;
	camera->speed = 2.5f;
	camera->sensitivity = 0.1f;
}

void Camera_Update(Camera* camera, float deltaTime) {
	//adding this later
}
