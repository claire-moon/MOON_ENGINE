#include "camera.h"
#include <cglm/cglm.h>

void Camera_Init(Camera* camera, vec3 position, vec3 up, float yaw, float pitch) {
	glm_vec3_copy(position, camera->position);
	glm_vec3_copy(up, camera->up);
	glm_vec3_copy(up, camera->worldUp);
	camera->yaw = -90.0f;
	camera->pitch = 0.0f;
	camera->speed = 2.5f;
	camera->sensitivity = 0.1f;
	camera->fov = 45.0f;

	Camera_UpdateVectors(camera);
}

void Camera_UpdateVectors(Camera* camera) {

	//calculate front vector
	vec3 front;
	front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
	front[1] = sin(glm_rad(camera->pitch));
	front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
	glm_vec3_normalize(front);
	glm_vec3_copy(front, camera->front);

	//calculate right vector
	vec3 right;
	glm_vec3_cross(camera->front, camera->worldUp, right);
	glm_vec3_normalize(right);
	glm_vec3_copy(right, camera->right);

	//calculate up vector
	vec3 up;
	glm_vec3_cross(camera->right, camera->front, up);
	glm_vec3_normalize(up);
	glm_vec3_copy(up, camera->up);
} 

//keyboard processing
void Camera_ProcessKeyboard(Camera* camera, int direction, float deltaTime) {
	float velocity = camera->speed * deltaTime;
	if (direction == 0) //forward
		glm_vec3_muladds(camera->front, velocity, camera->position);
	if (direction == 1) //backward
		glm_vec3_muladds(camera->front, -velocity, camera->position);
	if (direction == 2) //left
		glm_vec3_muladds(camera->right, -velocity, camera->position);
	if (direction == 3) //right
		glm_vec3_muladds(camera->right, velocity, camera->position);
}

//mouse processing
void Camera_ProcessMouseMovement(Camera* camera, float xoffset, float yoffset, int constrainPitch) {
	xoffset *= camera->sensitivity;
	yoffset *= camera->sensitivity;

	camera->yaw += xoffset;
	camera->pitch += yoffset;

	if (constrainPitch) {
		if (camera->pitch > 89.0f)
		    camera->pitch = 80.0f;
		if (camera->pitch < -89.0f)
		    camera->pitch = -89.0f;
	}

	Camera_UpdateVectors(camera);
}

void Camera_ProcessMouseScroll(Camera* camera, float yoffset) {
	camera->fov -= yoffset;
	if (camera->fov < 1.0f)
	    camera->fov = 1.0f;
	if (camera->fov > 45.0f)
	    camera->fov = 45.0f;
}

int Camera_CheckCollision(Camera* camera, vec3 position) {
	//less brain-dead implementation of collision code
	float roomMinX = -5.0f;
	float roomMaxX = 5.0f;
	float roomMinY = -5.0f;
	float roomMaxY = 5.0f;
	float roomMinZ = -5.0f;
	float roomMaxZ = 5.0f;

	//collision detection as followed (why is this code so beautiful..?)
	if (position[0] < roomMinX || position[0] > roomMaxX ||
	    position[1] < roomMinY || position[1] > roomMaxY ||
	    position[2] < roomMinZ || position[2] > roomMaxZ) {
		return 1; //COLLISION DETECTED !!
	}

	return 0; //NO COLLISION !!!
}
