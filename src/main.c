#include "core/engine.h"
#include "core/camera.h"
#include "core/input.h"
#include "core/renderer.h"
#include "shaders/shader.h"

int main(int argc, char* argv[]) {
	Engine engine;
	if (!Engine_Init(&engine, 800, 600, "MOON ENGINE")) {
		return 1;
	}

	//initialize camera
	Camera camera;
	vec3 cameraPos = {0.0f, 0.0f, 3.0f};
	vec3 cameraFront = {0.0f, 0.0f, -1.0f};
	vec3 cameraUp = {0.0f, 1.0f, 0.0f};
	Camera_Init(&camera, cameraPos, cameraFront, cameraUp);

	//load shaders
	GLuint shaderProgram = loadShaders("src/shaders/vertex_shader.glsl", "src/shaders/fragment_shader.glsl");
	if (shaderProgram == 0) {
		Engine_Shutdown(&engine);
		return 1;
	}

	//main loop
	Engine_Run(&engine);

	//clean up
	Engine_Shutdown(&engine);
	return 0;
}
