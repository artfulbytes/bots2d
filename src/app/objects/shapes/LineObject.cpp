#include "shapes/LineObject.h"
#include "components/Transforms.h"
#include "components/LineComponent.h"
#include "PhysicsWorld.h"

LineObject::LineObject(Scene *scene, const glm::vec4 &color, const glm::vec2 &start, const glm::vec2 &end, float width) :
    SceneObject(scene)
{
    const auto scaledStart = PhysicsWorld::scalePosition(start);
    const auto scaledEnd = PhysicsWorld::scalePosition(end);
    const auto scaledWidth = PhysicsWorld::scaleLengthNoAssert(width);

    m_transformComponent = std::make_unique<LineTransform>(scaledStart, scaledEnd, scaledWidth);
    const auto transform = static_cast<LineTransform *>(m_transformComponent.get());
    m_renderableComponent = std::make_unique<LineComponent>(transform, color);
}
