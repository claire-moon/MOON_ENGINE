#include <stdio.h>
#include <GL/glew.h>
#include <stdlib.h>

//read shader file into a string
char* readShaderFile(const char* filePath) {
	FILE* file = fopen(filePath, "r");
	if (!file) {
		printf("FAILED TO OPEN SHADER FILE!! %s\n", filePath);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* shaderSource = (char*)malloc(length + 1);
	fread(shaderSource, 1, length, file);
	shaderSource[length] = '\0';

	fclose(file);
	return shaderSource;
}

//shader compilation function
GLuint compileShader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	//check for compilation errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("SHADER COMPILATION FAILED: %s\n", infoLog);
		return 0;
	}

	return shader;
}

//func to load and link shaders into a program
GLuint loadShaders(const char* vertexPath, const char* fragmentPath) {
	char* vertexSource = readShaderFile(vertexPath);
	char* fragmentSource = readShaderFile(fragmentPath);

	if (!vertexSource || !fragmentSource) {
		free(vertexSource);
		free(fragmentSource);
		return 0;
	}

	//compile the shaders
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	free(vertexSource);
	free(fragmentSource);

	if (!vertexShader || !fragmentShader) {
		return 0;
	}

	//create shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//check for errors
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("SHADER PROGRAM LINK FAIL: %s\n", infoLog);
	}

	//clean up shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
