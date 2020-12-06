#include "QuadComponent.h"
#include "TexCoords.h"
#include "Transforms.h"
#include "Renderer.h"
#include "Texture.h"
#include "SpriteAnimation.h"

QuadComponent::QuadComponent(const glm::vec4& color)
    : m_color(color)
{
}

QuadComponent::QuadComponent(const std::string& textureFilepath) :
    m_texture(new Texture(textureFilepath))
{
}

QuadComponent::QuadComponent(const std::string &textureFilepath, SpriteAnimation &spriteAnimation) :
    m_texture(new Texture(textureFilepath)),
    m_texCoords(new TexCoords()),
    m_spriteAnimation(&spriteAnimation)
{
}

QuadComponent::~QuadComponent()
{
    delete m_texture;
    delete m_texCoords;
    delete m_spriteAnimation;
}

void QuadComponent::onFixedUpdate()
{
    if (m_spriteAnimation != nullptr) {
        m_spriteAnimation->onFixedUpdate();
        m_spriteAnimation->computeTexCoords(*m_texCoords);
    }
    const auto transformComp = m_parent->getTransform();
    const auto transform = dynamic_cast<const QuadTransform*>(transformComp);
    /* Transform of parent must be a quad transform! */
    assert(transform != nullptr);
    if (m_texture) {
        Renderer::drawQuad(transform->position, transform->size, transform->rotation, *m_texture, m_texCoords);
    } else {
        Renderer::drawQuad(transform->position, transform->size, transform->rotation, m_color);
    }
}
