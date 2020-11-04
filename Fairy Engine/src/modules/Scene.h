#pragma once

#include <string>
#include <vector>
#include <iostream> 
#include <fstream> 

#include <thread>
#include <future>

#include "../modules/Camera.h"
#include "../modules/Lights.h"
#include "../modules/Shader.h"

#include "../drawable/Model.h"

struct model
{
	std::string path;
	std::string name;

	glm::vec3 pos = glm::vec3(0.0f);
	Tags tag = Tags::STATIC;
};

class Scene
{
public:
	std::string name;
	std::vector<model> models;
	std::string shader_path;

public:
	Scene(std::string path);
	void Draw(float dt);

private:
	std::vector<std::unique_ptr<Model>> drawables;
	std::vector<std::future<void>> entities;

public:
	Camera camera;
	Lights lights;

private:
	std::unique_ptr<Shader> shader;
};

std::istream& operator>>(std::istream& is, Scene& scene);