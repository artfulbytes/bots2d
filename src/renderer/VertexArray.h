#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H_

class VertexBuffer;
class VertexBufferLayout;

/**
 * Wrapper around OpenGL vertex array object (VAO). A VAO ties a VertexBuffer to
 * a vertex buffer layout. So instead of having to separately bind a buffer and
 * its layout each time, we can just bind the VAO instead.
 */
class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void bind() const;
    void unbind() const;
private:
    unsigned int m_id;
};
#endif /* VERTEX_ARRAY_H_ */
