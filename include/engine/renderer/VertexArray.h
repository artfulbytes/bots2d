#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H_

class VertexBuffer;
class VertexBufferLayout;

/**
 * Wrapper around OpenGL vertex array object (VAO). A VAO contains VertexBuffer and
 * its layout.
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
