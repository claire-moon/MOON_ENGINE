#include "core/engine.h"
#include "core/camera.h"
#include "core/input.h"
#include "core/renderer.h"
#include "core/scenenode.h"
#include "core/map.h"
#include "shaders/shader.h"
#include <SDL2/SDL.h>

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
	Camera_Init(&camera, cameraPos, cameraUp, -90.0f, 0.0f);

	//load shaders
	GLuint shaderProgram = loadShaders("src/shaders/vertex_shader.glsl", "src/shaders/fragment_shader.glsl");
	if (shaderProgram == 0) {
		Engine_Shutdown(&engine);
		return 1;
	}

	//load map
	SceneNode* rootNode = Map_Load("src/maps/test.map");
	if (!rootNode) {
		Engine_Shutdown(&engine);
		return 1;
	}

	//main loop
	Engine_Run(&engine, &camera, shaderProgram, rootNode);
	
	//clean up
	SceneNode_Destroy(rootNode);
	Engine_Shutdown(&engine);
	return 0;
}
