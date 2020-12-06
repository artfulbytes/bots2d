#ifndef QUAD_COMPONENT_H_
#define QUAD_COMPONENT_H_

#include "RenderableComponent.h"

#include <glm/glm.hpp>
#include <string>

class TexCoords;
class Texture;
class SpriteAnimation;

class QuadComponent : public RenderableComponent
{
    public:
        QuadComponent(const glm::vec4& color);
        QuadComponent(const std::string& textureFilepath);
        QuadComponent(const std::string &textureFilepath, SpriteAnimation &spriteAnimation);
        ~QuadComponent();

        void onFixedUpdate() override;
    private:
        glm::vec4 m_color;
        Texture *const m_texture = nullptr;
        TexCoords *const m_texCoords = nullptr;
        SpriteAnimation *const m_spriteAnimation = nullptr;
};

#endif /* QUAD_COMPONENT_H_ */
