#ifndef ENGINE_H
#define ENGINE_H

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
void Engine_Run (Engine* engine);

#endif
