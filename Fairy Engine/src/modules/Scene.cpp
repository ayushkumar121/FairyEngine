#include "Scene.h"

static std::mutex entityMutex;
static void load_model(std::vector<std::unique_ptr<Model>>* drawables, model m)
{
	std::lock_guard<std::mutex> lock(entityMutex);
	drawables->push_back(std::make_unique<Model>(m.path, m.pos, m.tag));
}

Scene::Scene(std::string path)
{
	std::fstream file(path.c_str(), std::ios::in | std::ios::out);
	file >> *(this);

	for (size_t i = 0; i < models.size(); i++)
	{
		entities.push_back(std::async(std::launch::async, load_model, &drawables, models[i]));
	}

	shader = std::make_unique<Shader>(shader_path);
	file.close();
}

void Scene::Draw(float dt)
{
	shader->Bind();

	lights.Bind();
	camera.Bind();

	for (auto& d : drawables)
	{
		if (d->getTag() == Tags::COLLECTIBLE)
			d->Rotate(glm::vec3(0.0f, 0.0f, dt));

		d->Draw();
	}

	camera.SpawnControlWindow();

	if (ImGui::Begin("Debug"))
	{
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	}
	ImGui::End();

	shader->Unbind();
}

std::istream& operator>>(std::istream& is, Scene& scene)
{
	model m;
	char ch;

	std::string line;

	std::getline(is, scene.name);
	std::getline(is, scene.shader_path);

	while (std::getline(is, line))
	{
		if (line.front() == '-')
		{
			std::getline(is, m.name);
			line.erase(line.begin());
			m.path = line;

			is >> m.pos.x;
			is >> m.pos.y;
			is >> m.pos.z;

			is >> ch;

			switch (ch)
			{
			case 'S':
				m.tag = Tags::STATIC;
				break;

			case 'I':
				m.tag = Tags::INTERACTALE;
				break;

			case 'C':
				m.tag = Tags::COLLECTIBLE;
				break;

			default:
				break;
			}

			scene.models.push_back(m);
		}
		else if (line.front() == '@')
		{
			is >> scene.camera.position.x;
			is >> scene.camera.position.y;
			is >> scene.camera.position.z;

			is >> scene.camera.fov;
			is >> scene.camera.pitch;
			is >> scene.camera.roll;
			is >> scene.camera.yaw;

			for (size_t i = 0; i < 2; i++)
			{
				is >> scene.lights.lightData.lightDir[i].x;
				is >> scene.lights.lightData.lightDir[i].y;
				is >> scene.lights.lightData.lightDir[i].z;

				is >> scene.lights.lightData.lightColor[i].x;
				is >> scene.lights.lightData.lightColor[i].y;
				is >> scene.lights.lightData.lightColor[i].z;

				is >> scene.lights.lightData.lightIntensity[i];
			}
		}
	}

	return is;
}
