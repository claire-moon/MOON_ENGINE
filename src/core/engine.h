#ifndef ENGINE_H
#define ENGINE_H

#include "camera.h"
#include "scenenode.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <cglm/cglm.h>

typedef struct {
	SDL_Window* window;
	SDL_GLContext glContext;
	int running;
} Engine;

int Engine_Init(Engine* engine, int width, int height, const char* title);
void Engine_Shutdown(Engine* engine);
void Engine_Run (Engine* engine, Camera* camera, GLuint shaderProgram, SceneNode* rootNode);

#endif
