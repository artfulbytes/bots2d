#ifndef INDEX_BUFFER_H_
#define INDEX_BUFFER_H_

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
