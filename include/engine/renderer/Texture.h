#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <string>

class Texture
{
public:
    Texture(const std::string& textureName);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() const { return m_width; }
    inline int getHeight() const { return m_height; }

private:
    unsigned int m_id = 0;
    std::string m_filepath = nullptr;
    unsigned char* m_localBuffer = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_bpp = 0;

};
#endif /* TEXTURE_H_ */
