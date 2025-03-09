#include "input.h"
#include "camera.h"
#include <SDL2/SDL.h>

//define movement dir(s)
#ifndef FORWARD
#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#endif

void Input_HandleEvents(Engine* engine, Camera* camera, float deltaTime) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			engine->running = 0;
		} else if (event.type == SDL_MOUSEMOTION) {
			float xoffset = event.motion.xrel; //x-axis
			float yoffset = -event.motion.yrel; //inverted y-axis
			Camera_ProcessMouseMovement(camera, xoffset, yoffset, 1);
		} else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_w:
					Camera_ProcessKeyboard(camera, FORWARD, deltaTime);
					break;
				case SDLK_s:
					Camera_ProcessKeyboard(camera, BACKWARD, deltaTime);
					break;
				case SDLK_a:
					Camera_ProcessKeyboard(camera, LEFT, deltaTime);
					break;
				case SDLK_d:
					Camera_ProcessKeyboard(camera, RIGHT, deltaTime);
					break;
			}
		}
	}
}
