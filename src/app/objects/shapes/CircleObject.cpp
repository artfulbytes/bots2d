#include "shapes/CircleObject.h"
#include "components/Transforms.h"
#include "components/CircleComponent.h"
#include "components/HollowCircleComponent.h"
#include "Scene.h"

CircleObject::CircleObject(Scene *scene, const glm::vec4 &color,
                           const Body2D::Specification *spec,
                           const glm::vec2 &position, float radius) :
    SceneObject(scene)
{
    m_transformComponent = std::make_unique<CircleTransform>(position, radius, 0.0f);
    auto transform = static_cast<CircleTransform *>(m_transformComponent.get());
    m_renderableComponent = std::make_unique<CircleComponent>(transform, color);
    if (spec != nullptr) {
        m_physicsComponent = std::make_unique<Body2D>(*(scene->getPhysicsWorld()), transform, *spec);
    }
}

CircleObject::CircleObject(Scene *scene, const glm::vec4 &fillColor, const glm::vec4 &borderColor,
                           const glm::vec2 &position, float innerRadius, float outerRadius) :
    SceneObject(scene)
{
    m_transformComponent = std::make_unique<HollowCircleTransform>(position, innerRadius, outerRadius);
    auto transform = static_cast<HollowCircleTransform *>(m_transformComponent.get());
    m_renderableComponent = std::make_unique<HollowCircleComponent>(transform, fillColor, borderColor);
}

CircleObject::~CircleObject()
{
}
