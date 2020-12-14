#include "CircleObject.h"
#include "Transforms.h"
#include "CircleComponent.h"
#include "HollowCircleComponent.h"

CircleObject::CircleObject(Scene *scene, const glm::vec4 &color, const glm::vec2 &position, float radius) :
    SceneObject(scene)
{
    m_transformComponent = std::make_unique<CircleTransform>(position, radius);
    auto transform = static_cast<CircleTransform *>(m_transformComponent.get());
    m_renderableComponent = std::make_unique<CircleComponent>(transform, color);
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
