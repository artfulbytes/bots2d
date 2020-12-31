#ifndef RECT_COMPONENT_H_
#define RECT_COMPONENT_H_

#include "components/RenderableComponent.h"

#include <glm/glm.hpp>
#include <string>
#include <memory>

struct TexCoords;
class Texture;
class SpriteAnimation;
class RectTransform;
class CircleTransform;

/**
 * Renders a rectangle with single color or texture (and animation).
 *
 * It can't be used directly in a Scene, instead it must be assigned to
 * a Scene object to be updated each simulation iteration.
 */
class RectComponent : public RenderableComponent
{
public:
    RectComponent(const RectTransform *transform, const glm::vec4& color);
    RectComponent(const RectTransform *transform, const std::string &textureName, SpriteAnimation *spriteAnimation = nullptr);
    RectComponent(const CircleTransform *transform, const std::string &textureName, SpriteAnimation *spriteAnimation = nullptr);
    ~RectComponent();

    void onFixedUpdate() override;
private:
    const RectTransform *const m_quadTransform = nullptr;
    const CircleTransform *const m_circleTransform = nullptr;
    glm::vec4 m_color;
    const std::unique_ptr<Texture> m_texture;
    const std::unique_ptr<TexCoords> m_texCoords;
    SpriteAnimation *const m_spriteAnimation = nullptr;
};

#endif /* RECT_COMPONENT_H_ */
