#include "Texture.h"
#include "GLError.h"
#include "AssetsHelper.h"

#include <glad/gl.h>
#include "stb_image.h"
#include <cassert>
#include <iostream>

Texture::Texture(const std::string& textureName) :
    m_filepath(AssetsHelper::getTexturePath(textureName))
{
    /* Flips texture upside down because OpenGl expects pixels to
       start at the bottom left */
    stbi_set_flip_vertically_on_load(1);
    /* 4 channels since RBGA */
    m_localBuffer = stbi_load(m_filepath.c_str(), &m_width, &m_height, &m_bpp, 4);

    if (m_localBuffer == nullptr) {
        std::cout << "Could not find texture: " << m_filepath << std::endl;
        assert(m_localBuffer != nullptr);
    }

    GLCall(glGenTextures(1, &m_id));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_id));

    /* Resampling/scaling paramters filter etc...
     * You MUST specify this otherwise you get black texture.. */
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    /* Send the texture to OpenGl (allocate space on GPU)
     * First format is how OpenGL stores it (GL_RGBA8)
     * Second format is the format of the data we supply (GL_RGBA) */
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
    unbind();

    if (m_localBuffer) {
        stbi_image_free(m_localBuffer);
    }
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_id));
}

void Texture::bind(unsigned int slot) const
{
    /* Select which texture slot to use */
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    /* Bind texture to that slot */
    GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
}

void Texture::unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
