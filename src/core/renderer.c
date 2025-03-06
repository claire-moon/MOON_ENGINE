#include "renderer.h"
#include "../shaders/shader.h"

void Renderer_Render(Engine* engine, Camera* camera, GLuint shaderProgram) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//use the shader program
	glUseProgram(shaderProgram);

	//pass camera matrices to shader
	//and create view matrix
	mat4 view;
	vec3 center;
	glm_vec3_add(camera->position, camera->front, center); //calculate center point
	glm_lookat(camera->position, center, camera->up, view);

	//create projection matrix
	mat4 projection;
	float aspectRatio = 800.0f / 600.0f; 
	glm_perspective(glm_rad(camera->fov), aspectRatio, 0.1f, 100.0f, projection);


	//pass view matrix to shader
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);

	//pass projection matrix to shader
	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)projection);

	//render scene
	//(implementing this later)
	
	//swap buffers
	SDL_GL_SwapWindow(engine->window);
}
