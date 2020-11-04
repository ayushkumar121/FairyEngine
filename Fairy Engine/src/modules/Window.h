#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

class Window
{
private:
	GLFWwindow* window;

	int width;
	int height;

public:
	Window(int width, int height);
	~Window();

	void StartFrame();
	void EndFrame();

	GLFWwindow* Get();
};

