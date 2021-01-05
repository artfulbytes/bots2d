#include "IndexBuffer.h"
#include <glad/gl.h>
#include "GLError.h"
#include <cassert>

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
    : m_count(count)
{
    GLCall(glGenBuffers(1, &m_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    assert(sizeof(GLuint) == sizeof(unsigned int));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
