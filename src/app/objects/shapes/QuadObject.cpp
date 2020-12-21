#include "shapes/QuadObject.h"
#include "components/Transforms.h"
#include "components/QuadComponent.h"
#include "Scene.h"

QuadObject::QuadObject(Scene *scene, const glm::vec4 &color,
                       const Body2D::BodySpec *bodySpec,
                       const glm::vec2 &position, const glm::vec2 &size, float rotation) :
    SceneObject(scene)
{
    m_transformComponent = std::make_unique<QuadTransform>(position, size, rotation);
    const auto transform = static_cast<QuadTransform *>(m_transformComponent.get());
    m_renderableComponent = std::make_unique<QuadComponent>(transform, color);
    if (bodySpec != nullptr) {
        m_physicsComponent = std::make_unique<Body2D>(*(scene->getPhysicsWorld()), transform, *bodySpec);
    }
}

QuadObject::QuadObject(Scene *scene, const std::string& textureFilepath, const SpriteAnimation::Params *animationParams,
                       const Body2D::BodySpec *bodySpec,
                       const glm::vec2 &position, const glm::vec2 &size, float rotation) :
    SceneObject(scene)
{
    m_transformComponent = std::make_unique<QuadTransform>(position, size, rotation);
    const auto transform = static_cast<QuadTransform *>(m_transformComponent.get());
    if (animationParams != nullptr) {
        m_animation = std::make_unique<SpriteAnimation>(*animationParams);
    }
    m_renderableComponent = std::make_unique<QuadComponent>(transform, textureFilepath, m_animation.get());
    if (bodySpec != nullptr) {
        m_physicsComponent = std::make_unique<Body2D>(*(scene->getPhysicsWorld()), transform, *bodySpec);
    }
}
