#include "input.h"
#include "camera.h"

void Input_HandleEvents(Engine* engine) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			engine->running = 0;
		}

		//handle other input events (adding later)

	}
}
