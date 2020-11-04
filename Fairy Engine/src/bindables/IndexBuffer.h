#pragma once
#include <GL/glew.h>

class IndexBuffer
{
private:
	unsigned int id;
	unsigned int count;

public:
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount()
	{
		return count;
	}
};