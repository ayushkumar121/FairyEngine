#include "Lights.h"
#include <string>

Lights::Lights()
{
	Reset();
}

void Lights::Bind()
{
	glUniform3fv(8, 2, glm::value_ptr(lightData.lightDir[0]));
	glUniform3fv(10, 2, glm::value_ptr(lightData.lightColor[0]));
	glUniform1fv(12, 2, &lightData.lightIntensity[0]);
}

void Lights::SpawnControlWindow()
{
	char b[33];

	for (int i = 0; i < 2; i++)
	{
		_itoa_s(i, b, 10);

		if (ImGui::Begin(std::string(std::string("Light") + b).c_str()))
		{
			ImGui::SliderAngle("X", &lightData.lightDir[i].x);
			ImGui::SliderAngle("Y", &lightData.lightDir[i].y);
			ImGui::SliderAngle("Z", &lightData.lightDir[i].z);

			ImGui::ColorEdit3("Light Color", &lightData.lightColor[i].x);
			ImGui::DragFloat("Light Intensity", &lightData.lightIntensity[i], 0.05f, 0.0f, 5.0f);

			if (ImGui::Button("Reset"))
			{
				Reset();
			}
		}

		ImGui::End();
	}
}

void Lights::Reset()
{
	lightData = {
		{
			glm::vec3(0.0f),
			glm::vec3(0.0f)
		},
		{
			glm::vec3(1.0f),
			glm::vec3(1.0f)
		},
		{
			1.0f,
			1.0f
		}
	};
}
