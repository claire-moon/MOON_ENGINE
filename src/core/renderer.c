#include "renderer.h"
#include "shader.h"

void Renderer_Render(Engine* engine) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//scene rendering (implementing later)
	
	//swap buffers
	SDL_GL_SwapWindow(engine->window);
}
