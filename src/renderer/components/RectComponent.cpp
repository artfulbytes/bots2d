#include "components/RectComponent.h"
#include "Renderer.h"
#include "components/Transforms.h"
#include "TexCoords.h"
#include "Texture.h"
#include "SpriteAnimation.h"

RectComponent::RectComponent(const RectTransform *transform, const glm::vec4& color) :
    m_quadTransform(transform),
    m_color(color)
{
}

RectComponent::RectComponent(const RectTransform *transform, const std::string &textureName, SpriteAnimation *spriteAnimation) :
    m_quadTransform(transform),
    m_texture(std::make_unique<Texture>(textureName)),
    m_texCoords(std::make_unique<TexCoords>()),
    m_spriteAnimation(spriteAnimation)
{
}

RectComponent::RectComponent(const CircleTransform *transform, const std::string &textureName, SpriteAnimation *spriteAnimation) :
    m_circleTransform(transform),
    m_texture(std::make_unique<Texture>(textureName)),
    m_texCoords(std::make_unique<TexCoords>()),
    m_spriteAnimation(spriteAnimation)
{
}

RectComponent::~RectComponent()
{
}

void RectComponent::onFixedUpdate()
{
    if (m_enabled == false) {
        return;
    }

    if (m_spriteAnimation != nullptr) {
        m_spriteAnimation->onFixedUpdate();
        m_spriteAnimation->computeTexCoords(*m_texCoords);
    }

    glm::vec2 size;
    glm::vec2 position;
    float rotation = 0.0f;
    if (m_quadTransform != nullptr) {
        size = m_quadTransform->size;
        position = m_quadTransform->position;
        rotation = m_quadTransform->rotation;
    } else if (m_circleTransform != nullptr) {
        size = glm::vec2{ 2 * m_circleTransform->radius, 2 * m_circleTransform->radius };
        position = m_circleTransform->position;
        rotation = m_circleTransform->rotation;
    } else {
        assert(0);
    }

    if (m_texture) {
        Renderer::drawRect(position, size, rotation, *m_texture.get(), m_texCoords.get());
    } else {
        Renderer::drawRect(position, size, rotation, m_color);
    }
}
