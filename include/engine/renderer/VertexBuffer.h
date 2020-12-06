#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

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
    const int m_glDrawType;
    const unsigned int m_size;
};

#endif /* VERTEX_BUFFER_H_ */
