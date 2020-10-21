#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

private:
    unsigned int m_id;
};

#endif /* VERTEX_BUFFER_H_ */
