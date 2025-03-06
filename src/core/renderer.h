#ifndef RENDERER_H
#define RENDERER_H

#include "engine.h"
#include "camera.h"

void Renderer_Render(Engine* engine, Camera* camera, GLuint shaderProgram);

#endif
