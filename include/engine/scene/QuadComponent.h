#ifndef QUAD_COMPONENT_H_
#define QUAD_COMPONENT_H_

#include "RenderableComponent.h"

#include <glm/glm.hpp>
#include <string>

class TexCoords;
class Texture;
class SpriteAnimation;
class QuadTransform;

class QuadComponent : public RenderableComponent
{
public:
    QuadComponent(const QuadTransform *transform, const glm::vec4& color);
    QuadComponent(const QuadTransform *transform, const std::string &textureFilepath, SpriteAnimation *spriteAnimation = nullptr);
    ~QuadComponent();

    void onFixedUpdate() override;
private:
    const QuadTransform *const m_transform = nullptr;
    glm::vec4 m_color;
    const std::unique_ptr<Texture> m_texture;
    const std::unique_ptr<TexCoords> m_texCoords;
    SpriteAnimation *const m_spriteAnimation = nullptr;
};

#endif /* QUAD_COMPONENT_H_ */
