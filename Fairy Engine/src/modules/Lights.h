#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

class Lights
{
public:
	struct LightData
	{
		glm::vec3 lightDir[2];
		glm::vec3 lightColor[2];
		float lightIntensity[2];
	} lightData;

public:
	Lights();
	void Bind();

	void SpawnControlWindow();

private:
	void Reset();	
};

