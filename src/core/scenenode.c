#include "scenenode.h"
#include <stdlib.h>

SceneNode* SceneNode_Create() {
	SceneNode* node = (SceneNode*)malloc(sizeof(SceneNode));
	if (node) {
		glm_mat4_identity(node->transform);
		node->parent = NULL;
		node->children = NULL;
		node->childCount = 0;
		node->isVisible = true;
	}
	return node;
}

void SceneNode_Destroy(SceneNode* node) {
	if (node) {
		for (int i = 0; i < node->childCount; i++) {
			SceneNode_Destroy(node->children[i]);
		}
		free(node->children);
		free(node);
	}
}

void SceneNode_AddChild(SceneNode* parent, SceneNode* child) {
	if (parent && child) {
		parent->childCount++;
		parent->children = (SceneNode**)realloc(parent->children, parent->childCount * sizeof(SceneNode*));
		parent->children[parent->childCount - 1] = child;
		child->parent = parent;
	}
}

void SceneNode_RemoveChild(SceneNode* parent, SceneNode* child) {
	if (parent && child) {
		for (int i = 0; i < parent->childCount; i++) {
			if (parent->children[i] == child) {
				
				//shift remaining children
				for (int j = i; j < parent->childCount - 1; j++) {
					parent->children[j] = parent->children[j + 1];
				}
				parent->childCount--;
				parent->children = (SceneNode**)realloc(parent->children, parent->childCount * sizeof(SceneNode*));
				break;
			}
		}
	}
}

void SceneNode_UpdateTransform(SceneNode* node, mat4 parentTransform) {
	if (node) {
		mat4 globalTransform;
		glm_mat4_mul(parentTransform, node->transform, globalTransform);

		//update children
		for (int i = 0; i < node->childCount; i++) {
			SceneNode_UpdateTransform(node->children[i], globalTransform);
		}
	}
}
