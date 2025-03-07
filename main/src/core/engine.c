#include "engine.h"
#include "camera.h"
#include "input.h"
#include "renderer.h"

///initializing the engine
int Engine_Init (Engine* engine, int width, int height, const char* title) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL failed the init, innit! ERROR: %s\n", SDL_GetError());
		return 0;
	}

	//OPENGL setup
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //enable double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); //depth buffer
						    //
	//create window (in OPENGL)
	engine->window = SDL_CreateWindow (
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
		);

	if (engine->window == NULL) {
		printf("Window not created!! FUCK!!! ERROR: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	//create OPENGL context
	engine->glContext = SDL_GL_CreateContext(engine->window);
	if (engine->glContext == NULL) {
		printf("OPENGL FAILED!!! GIVE UP HOPE! ERROR: %s\n", SDL_GetError());
		SDL_DestroyWindow(engine->window);
		SDL_Quit();
		return 0;
	}

	//init GLEW innit!!
	glewExperimental = GL_TRUE; //allow for modern features
	if (glewInit() != GLEW_OK) {
		printf("GLEW FAILED INIT!!! NOOOO!!!!!\n");
		SDL_DestroyWindow(engine->window);
		SDL_Quit();
		return 0;
	}

	//enable depth testing
	glEnable(GL_DEPTH_TEST);

	engine->running = 1;
	return 1;
}

void Engine_Shutdown(Engine* engine) {
	SDL_GL_DeleteContext(engine->glContext);
	SDL_DestroyWindow(engine->window);
	SDL_Quit();
}

void Engine_Run(Engine* engine, Camera* camera, GLuint shaderProgram, SceneNode* rootNode) {
	Uint32 previousTime = SDL_GetTicks();
	while (engine->running) {
		//calculate delta time
		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - previousTime) / 1000.0f;
		previousTime = currentTime;

		//handle input and render
		Input_HandleEvents(engine, camera, deltaTime);
		Renderer_Render(engine, camera, shaderProgram, rootNode);
	}
}
