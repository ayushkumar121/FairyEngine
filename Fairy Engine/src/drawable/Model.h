#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include <filesystem>

#include "../bindables/VertexBuffer.h"
#include "../bindables/IndexBuffer.h"
#include "../bindables/Texture.h"

struct Vertex 
{
    glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 texCoords;
};

struct Material
{
	glm::vec3 albedo;

	float roughness;
	float mettalic;

	std::string albedoMap;
	bool hasAlbedoMap = false;
};

enum class Tags
{
	STATIC,
	INTERACTALE,
	COLLECTIBLE
};

class Mesh
{
private:
	unsigned int vao;
	glm::mat4 transform;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material mat);
	~Mesh();

	void Draw(glm::mat4 appliedTransform);

	glm::mat4 GetTransform() const;
	void SetTransform(glm::mat4 baseTransform);

private:
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer>  indexBuffer;
	std::unique_ptr<Texture> albedoMap;

	Material mat;
};

class Model
{
private:
	Assimp::Importer importer;
	const aiScene* scene;

	struct Transform
	{
		glm::vec3 position;
		glm::vec3 rotation;
	} transform;

public:
	Model(std::string path, glm::vec3 pos, Tags tag = Tags::STATIC);
	void Translate(glm::vec3 translation);
	void Rotate(glm::vec3 rotation);

	void Draw();

	Transform getTransform();
	Tags getTag();

private:
	void processNode(aiNode* node, const aiScene* scene);
	std::unique_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
	void SetTransform(glm::mat4 modelMatrix);

private:
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::string directory;
	Tags tag;

	bool initialized = false;
	bool bad_model = false;
};