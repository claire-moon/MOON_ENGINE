#include "scenenode.h"
#include <stdlib.h>

SceneNode* SceneNode_Create() {
	SceneNode* node = (SceneNode*)malloc(sizeof(SceneNode));
	if (node) {
		glm_mat4_identity(node->transform);
		glm_mat4_identity(node->globalTransform);
		node->parent = NULL;
		node->children = NULL;
		node->childCount = 0;
		node->textureID = 0;
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

		if (parent == child) {
			printf("ERROR: TRIED TO ADD NODE %p AS ITS OWN CHILD! FREAK!\n", parent);
			return;
		}

		//prevent circular references in hierarchy
		SceneNode* ancestor = parent;
		while (ancestor) {
			if (ancestor == child) {
				printf("ERROR: CIRCULAR REFERENCE DETECTED! NODE %p IS ALREADY ANCESTOR.\n", child);
				return;
			}
			ancestor = ancestor->parent;
		}

		parent->childCount++;
		parent->children = (SceneNode**)realloc(parent->children, parent->childCount * sizeof(SceneNode*));
		parent->children[parent->childCount - 1] = child;
		child->parent = parent;

		printf("ADDED CHILD NODE TO PARENT!\nPARENT HAS %d CHILDREN\n", parent->childCount);

		//PRINT ROOT NODE STRUCT
		printf("ROOT: %p, CHILD: %d\n", parent, parent->childCount);
		for (int i = 0; i < parent->childCount; i++) {
			printf("CHILD %d: %p (TEX ID: %d)\n", i, parent->children[i], parent->children[i]->textureID);
		}
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
		glm_mat4_copy(globalTransform, node->globalTransform);

		//update children
		for (int i = 0; i < node->childCount; i++) {
			SceneNode_UpdateTransform(node->children[i], globalTransform);
		}
	}
}
