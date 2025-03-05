#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <cglm/cglm.h>
#include <math.h>

GLuint loadShaders(const char* vertexPath, const char* fragmentPath);

int main(int argc, char* argv[]) {
	//initialize SDL with SDL_Init() while passing the proper command to
	//use video features. also: debug shit
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL failed the init, innit! ERROR: %s\n", SDL_GetError());
		return 1; 
	}

	//OPENGL setup
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //enables double buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // depth buffer
	
	//creates the window. hopefully lol. (with added OPENGL support)
	SDL_Window* window = SDL_CreateWindow(
			"MOON ENGINE",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			800,
			600,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
			);
	//WOO DEBUG
	if (window == NULL) {
		printf("Window not created!! FUCK!!! ERROR: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}	

	//create OpenGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL) {
		printf("OPENGL FAILED!!! GIVE UP HOPE! ERROR: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	//init GLEW innit
	glewExperimental = GL_TRUE; //allows for more modern features
	if (glewInit() != GLEW_OK) {
		printf("GLEW FAILED INIT!!! NOOO!!! \n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	//enable depth testing
	glEnable(GL_DEPTH_TEST);

	GLuint shaderProgram = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");
	if (shaderProgram == 0) {
		printf("FAILED TO LOAD SHADERS!!\n");
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	//vertex data for cube (positions + normals)
	float vertices[] = {
		//positions	     //normals
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
	 	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,
 		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
	 	-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 0.0f
	};

	//indices for cube
	unsigned int indices[] = {
		0, 1, 2,  2, 3, 0,
		4, 5, 6,  6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
	        16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20	
	};

	//creates VBO, VAO, and EBO
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind VAO
	glBindVertexArray(VAO);

	//bind and set VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind and set EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set vertex attribute pointers
	
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//unbind VAO
	glBindVertexArray(0);
	
	//set up transofmation matrices
	mat4 model;
	glm_mat4_identity(model);

	mat4 view;
	vec3 eye = {0.0f, 0.0f, 3.0f};
	vec3 center = {0.0f, 0.0f, 0.0f};
	vec3 up = {0.0f, 1.0f, 0.0f};
	glm_lookat(eye, center, up, view);

	//i forgot to put this in lol
	mat4 projection;
	glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

	//light position
	vec3 lightPos = {1.2f, 1.0f, 2.0f};
	GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");

	//initialize the main loop
	int running = 1;
	SDL_Event event;
	float angle = 0.0f; //rotation angle
	
	while (running) {
		//check to see if there are any events in the queue, and if
		//there is an event store that in the variable.
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
					}		

				}
			
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //set clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//use shader program
		glUseProgram(shaderProgram);

		//update rotation angle
		angle += 0.01f;
		if (angle > 360.0f) {
			angle -= 360.f;
		}

		//update model matrix (rotating the cube)
		glm_mat4_identity(model);
		glm_rotate(model, glm_rad(angle), (vec3){0.5f, 1.0f, 0.0f});

		//pass matrices to the shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model[0]);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection[0]);

		//pass light position to shader
		glUniform3f(lightPosLoc, lightPos[0], lightPos[1], lightPos[2]);

		//bind VAO and draw cube
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//swap buffers (double buffering)
		SDL_GL_SwapWindow(window);
	}

	//clean it all up!!!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}
	




