#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

/**
 * Wrapper around OpenGL vertex buffer object (VBO). A VBO holds vertex data such as
 * position and texture coordinates.
 */
class VertexBuffer
{
public:
    enum class DrawType { Static, Dynamic };
    VertexBuffer(const void* data, unsigned int size, DrawType drawType);
    ~VertexBuffer();
    void updateData(const void *data, unsigned int size);

    void bind() const;
    void unbind() const;

private:
    int glDrawType(DrawType drawType);

    unsigned int m_id;
    const unsigned int m_size;
    const int m_glDrawType;
};

#endif /* VERTEX_BUFFER_H_ */
