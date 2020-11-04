#include "Camera.h"
#include <algorithm>

template<typename T>
T wrap_angle(T theta) noexcept
{
	constexpr T twoPi = (T)2 * (T)PI_D;
	const T mod = (T)fmod(theta, twoPi);
	if (mod > (T)PI_D)
	{
		return mod - twoPi;
	}
	else if (mod < -(T)PI_D)
	{
		return mod + twoPi;
	}
	return mod;
}

Camera::Camera()
{
	Reset();
}

void Camera::Bind()
{
	glUniformMatrix4fv(0, 1, GL_FALSE, &GetProjection()[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &GetView()[0][0]);

	glUniform3f(7, position.x, position.y, position.z);
}

glm::mat4 Camera::GetProjection() const
{
	return glm::perspective(fov, 16.0f / 9.0f, 0.1f, 200.0f);
}

glm::mat4 Camera::GetView() const
{
	return glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f))
		*  glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0.0f, 0.0f, 1.0f)) 
		*  glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f))
		*  glm::translate(glm::mat4(1.0f), -position);
}

void Camera::Translate(glm::vec3 translation)
{
	auto t =  glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f)) 
		* glm::vec4(translation, 1.0f);

	position += glm::vec3(t.x, t.y, -t.z) * speed;
}

void Camera::Rotate(float dx, float dy)
{
	yaw = wrap_angle(yaw + dx * angular_speed);
	pitch = std::clamp(pitch + dy * angular_speed, -PI / 3, PI / 6);
}

void Camera::SpawnControlWindow()
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::DragFloat3("Position", &position.x, 0.5f, -500.0f, 500.0f);

		ImGui::SliderAngle("FOV", &fov, 10.0f, 120.0f);
		ImGui::SliderAngle("Pitch", &pitch);
		ImGui::SliderAngle("Roll", &roll);
		ImGui::SliderAngle("Yaw", &yaw);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}

	ImGui::End();
}

void Camera::Reset()
{
	position = glm::vec3(0.0f);
	fov = glm::radians(60.0f);
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}
