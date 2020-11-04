#include "Texture.h"
#include <iostream>


Texture::Texture(const std::string& path)
	: id(0), path(path), buffer(nullptr), width(0), height(0), bpp(0)
{
	stbi_set_flip_vertically_on_load(1);
	buffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Sending Texture to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (buffer)
		stbi_image_free(buffer);
	else
		std::cout << "TEXTURE LOAD FAILED [" << path << "]" << std::endl;
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}