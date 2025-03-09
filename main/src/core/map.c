#include "map.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SceneNode* Map_Load(const char* filePath, vec3* lightPos) {
	FILE* file = fopen(filePath, "r");
	if (!file) {
		printf("EPIC FAIL NO MAP LOADED: %s\n", filePath);
		return NULL;
	}

	SceneNode* rootNode = SceneNode_Create();
	char line[256];
	int lineNumber = 0;

	while (fgets(line, sizeof(line), file)) {
		lineNumber++;
		printf("READING LINE %d: %s", lineNumber, line);


		if (strncmp(line, "node", 4) == 0) {
			float x, y, z;
			float rx, ry, rz;
			float sx, sy, sz;
			char textureFile[256];
			int parsed = sscanf(line + 5, "%f %f %f %f %f %f %f %f %f %s", &x, &y, &z, &rx, &ry, &rz, &sx, &sy, &sz, textureFile);

			if (parsed != 10){
				printf("FAILED TO PARSE NODE ON LINE %d\n", lineNumber);
				continue;
			}

			SceneNode* node = SceneNode_Create();
			glm_translate(node->transform, (vec3){x, y, z});
			
			//apply rotations
			glm_rotate_z(node->transform, glm_rad(rz), node->transform);
			glm_rotate_y(node->transform, glm_rad(ry), node->transform);
			glm_rotate_x(node->transform, glm_rad(rx), node->transform);

			//apply scale
			glm_scale(node->transform, (vec3){sx, sy, sz});

			//load tex
			GLuint textureID;
			if (strcmp(textureFile, "noise") == 0) {
				textureID = load2DNoiseTexture();
				if (textureID == 0) {
					printf("FAILED TO LOSE NOISE TEX ON LINE %d\n", lineNumber);
					textureID = defaultTextureID;
				}
			}
			node->textureID = textureID;

			SceneNode_AddChild(rootNode, node);
			printf("ADDED NODE WITH TEX ID: %d\n", textureID);
		} else if (strncmp(line, "light", 5) == 0) {
			int parsed = sscanf(line + 6, "%f %f %f", &(*lightPos)[0], &(*lightPos)[1], &(*lightPos)[2]);
			if (parsed != 3) {
				printf("FAILURE TO PARSE LIGHT POS ON LINE %d\n", lineNumber);
			} else {
				printf("LOADED LIGHT POS: %.2f, %.2f, %.2f\n", (*lightPos)[0], (*lightPos)[1], (*lightPos)[2]);
			}
		}
	}

	printf("LOADED %d NODES FROM MAP FILE\n", rootNode->childCount);
	fclose(file);
	return rootNode;
}
