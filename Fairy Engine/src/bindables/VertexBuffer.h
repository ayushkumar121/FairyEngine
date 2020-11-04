#pragma once
#include <GL/glew.h>

#include <string>
#include <vector>


class VertexBuffer
{
private:
	unsigned int id;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void AddVertexLayout(int i, unsigned int count, unsigned int vertexSize, unsigned int offset);
};