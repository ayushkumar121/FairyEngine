#pragma once
#include<GL/glew.h>

#include <sstream>
#include <fstream>

#include "Camera.h"
#include "Lights.h"

class Shader
{
private:
	unsigned int id;
	std::string path;

public:
	Shader(const std::string& path);
	~Shader();

	void Bind() const;
	void Unbind() const;

private:
	struct ShaderProgramSources
	{
		std::string VertexShaderSource;
		std::string FragmentShaderSource;
	};

	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSources ParseSource(const std::string& path);
};