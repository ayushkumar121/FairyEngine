#pragma once
#include <GL/glew.h>
#include <stb_image/stb_image.h>

#include <iostream>
#include <vector>

#include "../bindables/VertexBuffer.h"

#include "../modules/Camera.h"
#include "../modules/Shader.h"

class Skybox
{
private:
	unsigned int vao;
	std::vector<std::string> faces =
	{
		"res/textures/skybox/right.png",
		"res/textures/skybox/left.png",
		"res/textures/skybox/top.png",
		"res/textures/skybox/bottom.png",
		"res/textures/skybox/front.png",
		"res/textures/skybox/back.png"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

public:
	Skybox();
	~Skybox();

	void Draw(Camera cam);

private:
	Shader shader;
	std::unique_ptr<VertexBuffer> vertexBuffer;

	unsigned int loadCubemap(std::vector<std::string> faces);
};