#pragma once
#include <GL/glew.h>
#include <string>

#include <stb_image/stb_image.h>

class Texture
{
private:
	unsigned int id;
	const std::string path;
	unsigned char* buffer;
	int width, height, bpp;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const
	{
		return width;
	}

	inline int GetHeight() const
	{
		return height;
	}
};