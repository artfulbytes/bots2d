#include "sensors/RangeSensorObject.h"
#include "components/RangeSensor.h"
#include "components/Transforms.h"
#include "components/LineComponent.h"

#include <glm/glm.hpp>

RangeSensorObject::RangeSensorObject(Scene *scene, const Specification &unscaledSpec, bool debugShow,
                                     const glm::vec2 unscaledStartPosition) :
    SceneObject(scene)
{
    LineTransform *transform = nullptr;
    if (debugShow) {
        m_transformComponent = std::make_unique<LineTransform>();
        transform = static_cast<LineTransform *>(m_transformComponent.get());
        glm::vec4 color(0.0f, 0.5f, 0.0f, 1.0f);
        m_renderableComponent = std::make_unique<LineComponent>(transform, color);
    }
    m_physicsComponent = std::make_unique<RangeSensor>(*m_physicsWorld, transform,
                                                       unscaledStartPosition, unscaledSpec.relativeAngle,
                                                       unscaledSpec.minDistance, unscaledSpec.maxDistance);
    m_rangeSensor = static_cast<RangeSensor *>(m_physicsComponent.get());
}

RangeSensorObject::~RangeSensorObject()
{
}

Body2D *RangeSensorObject::getBody() const
{
    return m_rangeSensor->getBody();
}

float *RangeSensorObject::getVoltageLine() const
{
    return m_rangeSensor->getVoltageLine();
}

void RangeSensorObject::onFixedUpdate(double stepTime)
{
}
