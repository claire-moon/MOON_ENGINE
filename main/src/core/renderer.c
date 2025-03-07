#include "renderer.h"
#include "../shaders/shader.h"
#include <math.h>
#include <stdlib.h>

//hopefully i can just declare this here otherwise i need to include this in
//a header file... ;(
GLuint cubemapTexture, skyboxVAO, skyboxVBO, skyboxEBO;

float skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
 	 1.0f,  1.0f,  1.0f,	 
};

unsigned int skyboxIndices[] = {
	//back
	0, 1, 2,
	2, 3, 0,

	//front
	4, 5, 6,
	6, 5, 7,

	//left
	0, 1, 4,
	4, 5, 0,

	//right
	2, 3, 6,
	6, 3, 7,

	//bottom
	1, 4, 6,
	6, 2, 1,

	//top
	0, 3, 5,
	5, 3, 7,
};

//smooth interpolation function
float fade(float t) {
	return t * t * t * (t *(t * 6 - 15) + 10);
}

//linear interpolation function
float lerp(float a, float b, float t) {
	return a + t * (b - a);
}

//return a random gradient vector
float grad(int hash, float x, float y, float z) {
	int h = hash & 15;
	float u = h < 8 ? x : y;
	float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

//permutation table for perlin noise (Jesus Christ WHY did I TYPE THIS OUT BY HAND???)
int p[512] = {
152, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

//init the premutation table, innit!!
//void initPermutationTable() {
//	for (int i = 0; i < 256; i++) {
//		p[256 + i] = p[i];
//	}
//}

//perlin noise function
float perlin(float x, float y, float z) {
	//find the unit cube that contains the point
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	//find relative x, y, z of the point in the cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	//compute fade curves for x, y, z
	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	//hash coordinates of the eight cube corners
	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;
	
	// add blended results from eight cube corners
	float res = lerp(
		lerp(
			lerp(grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z), u),
			lerp(grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z), u),
			v
		),
		lerp(
			lerp(grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1), u),
			lerp(grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1), u),
			v
		),
		w
	);	
			return (res + 1.0f) / 2.0f;
	}

//generate noise texture for cubemap face
unsigned char* generateNoiseData(int width, int height, float scale, vec3 color) {
	unsigned char* data = (unsigned char*)malloc(width * height * 3);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float nx = (float)x / width;
			float ny = (float)y / height;
			float nz = 0.5f;

			float noise = perlin(nx * scale, ny * scale, nz * scale);
			noise = (noise + 1.0f) * 0.5f;

			int index = (y * width + x) * 3;
			data[index] = (unsigned char)(noise * color[0] * 255);
			data[index + 1] = (unsigned char)(noise * color[1] * 255);
			data[index + 2] = (unsigned char)(noise * color[2] * 255);
		}
	}
	return data;
}

//load cubemap tex
GLuint loadCubemap() {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width = 512, height = 512; //texture resolution
	float scale = 5.0f; //noise scale
	vec3 color = {1.0f, 0.8f, 0.6f}; //base color for noise
	
	for (unsigned int i = 0; i < 6; i++) {
		unsigned char* data = generateNoiseData(width, height, scale, color);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		free(data); //free data after uploading
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void Renderer_Init() {
	cubemapTexture = loadCubemap();

	//generate and bind VAO for the skybox
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);

	//now for VBO...
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	//...and EBO...
	glGenBuffers(1, &skyboxEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);

	//set up vertex attribute pointers for skybox
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind VAO
	glBindVertexArray(0);
}

void Renderer_RenderSceneNode(SceneNode* node, GLuint shaderProgram) {
	if (node && node->isVisible) {
		//pass the node transformation to the shader
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)node->transform);


		//bind the nodes texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, node->textureID);
		glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

		//render node mesh (mesh system implemented later)

		//render children
		for (int i = 0; i < node->childCount; i++) {
			Renderer_RenderSceneNode(node->children[i], shaderProgram);
		}
	}
}

//render skybox
void Renderer_RenderSkybox(GLuint shaderProgram, Camera* camera) {
	glDepthMask(GL_FALSE); //disable depth writing (always behind everything)
	glUseProgram(shaderProgram);

	//remove translation from view matrix (always appears at same distance)
	mat4 view;
	glm_mat4_copy(camera->view, view);
	view[3][0] = view[3][1] = view[3][2] = 0.0f; //remove translation
	
	mat4 projection;
	float aspectRatio = 800.0f / 600.0f;
	glm_perspective(glm_rad(camera->fov), aspectRatio, 0.1f, 100.0f, projection);

	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);

	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)projection);

	//bind cubemap tex
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);

	//render it!!
	glBindVertexArray(skyboxVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE); //re-enable depth writing
}

void Renderer_Render(Engine* engine, Camera* camera, GLuint shaderProgram, SceneNode* rootNode) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render skybox
	Renderer_RenderSkybox(shaderProgram, camera);

	//render scene graph
	if (rootNode) {
		mat4 identity;
		glm_mat4_identity(identity);
		SceneNode_UpdateTransform(rootNode, identity);
		Renderer_RenderSceneNode(rootNode, shaderProgram);
	}
	
	//swap buffers
	SDL_GL_SwapWindow(engine->window);
}
