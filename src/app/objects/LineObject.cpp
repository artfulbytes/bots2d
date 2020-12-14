#include "LineObject.h"
#include "Transforms.h"
#include "LineComponent.h"

LineObject::LineObject(Scene *scene, const glm::vec4 &color, const glm::vec2 &start, const glm::vec2 &end, float width) :
    SceneObject(scene)
{
    m_transformComponent = std::make_unique<LineTransform>(start, end, width);
    const auto transform = static_cast<LineTransform *>(m_transformComponent.get());
    m_renderableComponent = std::make_unique<LineComponent>(transform, color);
}
