#include "components/QuadComponent.h"
#include "Renderer.h"
#include "components/Transforms.h"
#include "TexCoords.h"
#include "Texture.h"
#include "SpriteAnimation.h"

QuadComponent::QuadComponent(const QuadTransform *transform, const glm::vec4& color) :
    m_transform(transform),
    m_color(color)
{
}

QuadComponent::QuadComponent(const QuadTransform *transform, const std::string &textureName, SpriteAnimation *spriteAnimation) :
    m_transform(transform),
    m_texture(std::make_unique<Texture>(textureName)),
    m_texCoords(std::make_unique<TexCoords>()),
    m_spriteAnimation(spriteAnimation)
{
}

QuadComponent::~QuadComponent()
{
}

void QuadComponent::onFixedUpdate()
{
    if (m_spriteAnimation != nullptr) {
        m_spriteAnimation->onFixedUpdate();
        m_spriteAnimation->computeTexCoords(*m_texCoords);
    }

    assert(m_transform != nullptr);
    if (m_texture) {
        Renderer::drawQuad(m_transform->position, m_transform->size, m_transform->rotation, *m_texture.get(), m_texCoords.get());
    } else {
        Renderer::drawQuad(m_transform->position, m_transform->size, m_transform->rotation, m_color);
    }
}
