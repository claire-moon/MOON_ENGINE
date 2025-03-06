#include "input.h"
#include "camera.h"

void Input_HandleEvents(Engine* engine, Camera* camera, float deltaTime) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				engine->running = 0;
				break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_w:
					Camera_ProcessKeyboard(camera, 0, deltaTime);
					break;
				case SDLK_s:
					Camera_ProcessKeyboard(camera, 1, deltaTime);
					break;
				case SDLK_a:
					Camera_ProcessKeyboard(camera, 2, deltaTime);
					break;
				case SDLK_d:
					Camera_ProcessKeyboard(camera, 3, deltaTime);	
			}
			break;

		case SDL_MOUSEMOTION:
			Camera_ProcessMouseMovement(camera, event.motion.xrel, -event.motion.yrel, 1);
			break;
		}
	}
}
