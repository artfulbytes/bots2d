#include "VertexBuffer.h"
#include "GLError.h"
#include <glad/gl.h>
#include <cassert>

VertexBuffer::VertexBuffer(const void* data, size_t size, VertexBuffer::DrawType drawType) :
    m_size(size), m_glDrawType(glDrawType(drawType))
{
    GLCall(glGenBuffers(1, &m_id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, m_glDrawType));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_id));
}

int VertexBuffer::glDrawType(VertexBuffer::DrawType drawType)
{
    switch (drawType)
    {
    case VertexBuffer::DrawType::Static: return GL_STATIC_DRAW;
    case VertexBuffer::DrawType::Dynamic: return GL_DYNAMIC_DRAW;
    }
    return 0;
}

void VertexBuffer::updateData(const void *data, size_t size)
{
    assert(size == m_size);
    bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, m_glDrawType));
    unbind();
}

void VertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
