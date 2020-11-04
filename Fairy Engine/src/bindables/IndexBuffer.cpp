#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
    : count(count)
{
    glGenBuffers(1, &id); // creating buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); // selecting/binding buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW); // copy data to the selected buffer
}

IndexBuffer::~IndexBuffer() 
{
    glDeleteBuffers(1, &id);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}