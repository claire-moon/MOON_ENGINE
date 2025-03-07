#ifndef SCENENODE_H
#define SCENENODE_H

#include <cglm/cglm.h>
#include <stdbool.h>
#include <GL/glew.h>

typedef struct SceneNode {
	mat4 transform; //local transformation matrix
	struct SceneNode* parent; //parent node
	struct SceneNode** children; //array of child nodes
	int childCount; //num of children
	bool isVisible; //whether node is visible
	GLuint textureID; //texture ID
} SceneNode;

//function prototypes
SceneNode* SceneNode_Create();
void SceneNode_Destroy(SceneNode* node);
void SceneNode_AddChild(SceneNode* parent, SceneNode* child);
void SceneNode_RemoveChild(SceneNode* parent, SceneNode* child);
void SceneNode_UpdateTransform(SceneNode* node, mat4 parentTransform);

#endif
