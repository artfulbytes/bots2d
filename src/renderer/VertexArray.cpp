#include "VertexArray.h"
#include <glad/gl.h>
#include "GLError.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_id));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_id));
}

/* There are two main ways to store vertex data:
 * Separate: Spread the vertex across data specific buffers (e.g. position, normal, texcoord)
 * Interleave: Store the vertex data in a single buffer.
 * Here we do it the interleaving way. */
void VertexArray::addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
{
    bind();
    vertexBuffer.bind();
    const auto& elements = layout.getElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
                                     layout.getStride(), reinterpret_cast<const void*>(offset)));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(m_id));
}

void VertexArray::unbind() const
{
    GLCall(glBindVertexArray(0));
}
