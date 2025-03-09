#ifndef RENDERER_H
#define RENDERER_H

#include "engine.h"
#include "camera.h"
#include "scenenode.h"
#include <GL/glew.h>
#include <cglm/cglm.h>

extern GLuint cubeVAO, cubeVBO, cubeEBO;

GLuint load2DNoiseTexture();
unsigned char* generateNoiseData(int width, int height, float scale, vec3 color);
GLuint loadCubemap();
void Renderer_Init();
void Renderer_RenderSceneNode(SceneNode* node, GLuint shaderProgram, GLuint VAO);
void Renderer_RenderSkybox(GLuint shaderProgram, Camera* camera);
void Renderer_Render(Engine* engine, Camera* camera, GLuint shaderProgram, SceneNode* rootNode, vec3 lightPos, GLuint VAO);
extern GLuint defaultTextureID;

#endif
