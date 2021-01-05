#ifndef VERTEX_BUFFER_LAYOUT_H_
#define VERTEX_BUFFER_LAYOUT_H_

#include <glad/gl.h>
#include <vector>
#include <cassert>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    bool normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_UNSIGNED_BYTE: return 1;
        }
        assert(false);
        return 0;
    }
};

/**
 * Wrapper around OpenGL vertex buffer layout, which labels the data in a
 * vertex buffer.
 */
class VertexBufferLayout
{
public:
    VertexBufferLayout() :
        m_stride(0)
    {
    }

    template<typename T>
    void push(unsigned int count)
    {
        static_assert(true);
    }

    inline const std::vector<VertexBufferElement> getElements() const { return m_elements; }
    inline unsigned int getStride() const { return m_stride; }

private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;
};

template<>
inline void VertexBufferLayout::push<float>(unsigned int count)
{
    m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}

template<>
inline void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

#endif /* VERTEX_BUFFER_LAYOUT_H_ */
