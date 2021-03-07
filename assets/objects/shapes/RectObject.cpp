#include "shapes/RectObject.h"
#include "components/Transforms.h"
#include "components/RectComponent.h"
#include "Scene.h"

RectObject::RectObject(Scene *scene, const glm::vec4 &color,
                       const Body2D::Specification *spec,
                       const glm::vec2 &position, const glm::vec2 &size, float rotation) :
    SceneObject(scene)
{
    m_transformComponent = std::make_unique<RectTransform>(position, size, rotation);
    const auto transform = static_cast<RectTransform *>(m_transformComponent.get());
    m_renderableComponent = std::make_unique<RectComponent>(transform, color);
    if (spec != nullptr) {
        m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, *spec);
    }
}

RectObject::RectObject(Scene *scene, const std::string& textureFilepath, const SpriteAnimation::Params *animationParams,
                       const Body2D::Specification *spec,
                       const glm::vec2 &position, const glm::vec2 &size, float rotation) :
    SceneObject(scene)
{
    m_transformComponent = std::make_unique<RectTransform>(position, size, rotation);
        const auto transform = static_cast<RectTransform *>(m_transformComponent.get());
    if (animationParams != nullptr) {
        m_animation = std::make_unique<SpriteAnimation>(*animationParams);
    }
    m_renderableComponent = std::make_unique<RectComponent>(transform, textureFilepath, m_animation.get());
    if (spec != nullptr) {
        m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, *spec);
    }
}

Body2D *RectObject::getBody() const
{
    return static_cast<Body2D *>(m_physicsComponent.get());
}
