#include "Shader.h"

Shader::Shader(const std::string& path)
	:path(path), id(0)
{
    ShaderProgramSources shaderSources = ParseSource(path);
    id = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, shaderSources.VertexShaderSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, shaderSources.FragmentShaderSource);

    glAttachShader(id, vs);
    glAttachShader(id, fs);

    glLinkProgram(id);
    glValidateProgram(id);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::Bind() const
{
    glUseProgram(id);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        glDeleteShader(id);
        return 0;
    }

    return id;
}

Shader::ShaderProgramSources Shader::ParseSource(const std::string& path)
{
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::ifstream stream(path);
    std::string line;
    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}
