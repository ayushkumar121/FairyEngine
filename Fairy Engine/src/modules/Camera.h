#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

class Camera
{
public:
	glm::vec3 position;
	
	float fov;
	float pitch;
	float roll;
	float yaw;
	
public:
	Camera();
	void Bind();

	glm::mat4 GetProjection() const;
	glm::mat4 GetView() const;

	void Translate(glm::vec3 translation);
	void Rotate(float dx, float dy);

	void SpawnControlWindow();

private:
	void Reset();

	float speed = 8.0f;
	float angular_speed = 0.001f;
};

