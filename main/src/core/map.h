#ifndef MAP_H
#define MAP_H

#include "scenenode.h"
#include <cglm/cglm.h>

SceneNode* Map_Load(const char* filePath, vec3* lightPos);

#endif
