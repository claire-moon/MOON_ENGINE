#include "map.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SceneNode* Map_Load(const char* filePath) {
	FILE* file = fopen(filePath, "r");
	if (!file) {
		printf("EPIC FAIL NO MAP LOADED: %s\n", filePath);
		return NULL;
	}

	SceneNode* rootNode = SceneNode_Create();
	char line[256];
	while (fgets(line, sizeof(line), file)) {
		if (strncmp(line, "node", 4) == 0) {
			float x, y, z;
			float rx, ry, rz;
			float sx, sy, sz;
			char textureFile[256];
			sscanf(line + 5, "%f %f %f %f %f %f %f %f %f %s", &x, &y, &z, &rx, &ry, &rz, &sx, &sy, &sz, textureFile);

			SceneNode* node = SceneNode_Create();
			glm_translate(node->transform, (vec3){1.0f, 0.0f, 0.0f});
			glm_rotate(node->transform, glm_rad(rx), (vec3){1.0f, 0.0f, 0.0f});
			glm_rotate(node->transform, glm_rad(ry), (vec3){0.0f, 1.0f, 0.0f});
			glm_rotate(node->transform, glm_rad(rz), (vec3){0.0f, 0.0f, 1.0f});
			glm_scale(node->transform, (vec3){sx, sy, sz});

			//load tex
			GLuint textureID;
			if (strcmp(textureFile, "noise") == 0) {
				unsigned char* noiseData = generateNoiseData(512, 512, 5.0f, (vec3){1.0f, 0.8f, 0.6f}); 
			} else {
					unsigned char* noiseData  = generateNoiseData(512, 512, 5.0f, (vec3){0.5f, 0.5f, 0.5f});
				}		
			node->textureID = textureID;

			SceneNode_AddChild(rootNode, node);
		}
	}
	fclose(file);
	return rootNode;
}
