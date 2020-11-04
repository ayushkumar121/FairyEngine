#include "Model.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material mat)
	:
	transform(1.0f),
	mat(mat)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vertexBuffer = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));

	vertexBuffer->AddVertexLayout(0, 3, sizeof(Vertex), offsetof(Vertex, Position));
	vertexBuffer->AddVertexLayout(1, 3, sizeof(Vertex), offsetof(Vertex, Normal));
	vertexBuffer->AddVertexLayout(2, 2, sizeof(Vertex), offsetof(Vertex, texCoords));

	indexBuffer = std::make_unique<IndexBuffer>(&indices[0], indices.size());
	
	if(mat.hasAlbedoMap)
		albedoMap = std::make_unique<Texture>(mat.albedoMap);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
}

void Mesh::Draw(glm::mat4 appliedTransform)
{	
	auto modelMatrix = appliedTransform * transform;

	glUniformMatrix4fv(2, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniform3f(4, mat.albedo.x, mat.albedo.y, mat.albedo.z); // material color
	glUniform1i(5, mat.hasAlbedoMap);
	glUniform1i(6, 0); // Texture slot

	glBindVertexArray(vao); 
	
	if (mat.hasAlbedoMap)
		albedoMap->Bind(0);

	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

glm::mat4 Mesh::GetTransform() const
{
	return transform;
}

void Mesh::SetTransform(glm::mat4 baseTransform)
{
	this->transform = baseTransform;
}

Model::Model(std::string path, glm::vec3 pos, Tags tag)
	:
	tag(tag),
	transform({
		pos,
		glm::vec3(0.0f, 0.0f, 0.0f)
	})
{
	scene = importer.ReadFile(path.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenSmoothNormals |
		aiProcess_GenUVCoords	
	);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP " << importer.GetErrorString() << std::endl;
		bad_model = true;
		return;
	}

	std::cout << "MODEL LOADED IN "
			  << " [" << path << "]" << std::endl;

	directory = path.substr(0, path.find_last_of('/'));
}

void Model::Translate(glm::vec3 translation)
{
	transform.position += translation;
}

void Model::Rotate(glm::vec3 rotation)
{
	transform.rotation += rotation;
}

Model::Transform Model::getTransform()
{
	return transform;
}

Tags Model::getTag()
{
	return tag;
}

void Model::Draw()
{
	if (!initialized && !bad_model)
	{
		processNode(scene->mRootNode, scene);
		initialized = true;
	}

	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i]->Draw(
			glm::translate(glm::mat4(1.0f), transform.position)
			* glm::rotate(glm::mat4(1.0f), transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), transform.rotation.y, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::rotate(glm::mat4(1.0f), transform.rotation.z, glm::vec3(0.0f, 1.0f, 0.0f))
		);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

std::unique_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	Material mat;
	Vertex ver;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	vertices.reserve(mesh->mNumVertices);
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		ver.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		ver.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		ver.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

		vertices.emplace_back(ver);
	}

	indices.reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		indices.emplace_back(face.mIndices[0]);
		indices.emplace_back(face.mIndices[1]);
		indices.emplace_back(face.mIndices[2]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		auto& material = *scene->mMaterials[mesh->mMaterialIndex];

		aiColor3D c(0.f, 0.f, 0.f);
		aiString alebdoMap;

		material.GetTexture(aiTextureType_DIFFUSE, 0, &alebdoMap);
		material.Get(AI_MATKEY_COLOR_DIFFUSE, c);

		std::string alebdoMapFile = directory + "/" + alebdoMap.C_Str();

		if (!std::filesystem::is_directory(alebdoMapFile) && std::filesystem::exists (alebdoMapFile))
		{
			mat.albedoMap = alebdoMapFile;
			mat.hasAlbedoMap = true;
		}

		mat.albedo = glm::vec3(c.r, c.g, c.b);
	}

	return std::make_unique<Mesh>(vertices, indices, mat);
}

void Model::SetTransform(glm::mat4 modelMatrix)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i]->SetTransform(modelMatrix);
}
