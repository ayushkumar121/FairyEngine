#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <thread>
#include <future>

#include "modules/Window.h"
#include "modules/TImer.h"
#include "modules/Scene.h"
#include "drawable/Skybox.h"

class App
{
public:
	App();
	int Go();
	~App();

private:
	void Loop(float dt);
	void Move(float dt);

private:
	Window window;
	Timer timer;
	Scene scene;
	Skybox skybox;

	double last_x;
	double last_y;

	std::vector<std::string> inventory;
	bool game_paused = false;
};

