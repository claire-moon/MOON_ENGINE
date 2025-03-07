#include "core/engine.h"
#include "core/camera.h"
#include "core/input.h"
#include "core/renderer.h"
#include "core/scenenode.h"
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

	//create scene graph
	SceneNode* rootNode = SceneNode_Create();
	SceneNode* childNode1 = SceneNode_Create();
	SceneNode* childNode2 = SceneNode_Create();

	//transformation setup
	glm_translate(rootNode->transform, (vec3){0.0f, 0.0f, -5.0f});
	glm_translate(childNode1->transform, (vec3){1.0f, 0.0f, 0.0f});
	glm_translate(childNode2->transform, (vec3){-1.0f, 0.0f, 0.0f});

	//add children to root node
	SceneNode_AddChild(rootNode, childNode1);
	SceneNode_AddChild(rootNode, childNode2);

	//main loop
	Engine_Run(&engine, &camera, shaderProgram, rootNode);
	
	//clean up
	SceneNode_Destroy(rootNode);
	Engine_Shutdown(&engine);
	return 0;
}
