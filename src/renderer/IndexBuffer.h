#ifndef INDEX_BUFFER_H_
#define INDEX_BUFFER_H_

/**
 * Wrapper around OpenGL index buffer. An index buffer indexes into
 * a vertex buffer, and prevents storage duplication when using the
 * same vertex multiple times.
 */
class IndexBuffer
{
public:
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline unsigned int getCount() const { return m_count; }

private:
    unsigned int m_id;
    unsigned int m_count;
};

#endif /* INDEX_BUFFER_H_ */
