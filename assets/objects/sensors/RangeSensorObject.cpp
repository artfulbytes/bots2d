#include "sensors/RangeSensorObject.h"
#include "components/RangeSensor.h"
#include "components/Transforms.h"
#include "components/LineComponent.h"

#include <glm/glm.hpp>

RangeSensorObject::RangeSensorObject(Scene *scene, const Specification &spec, bool debugDrawEnabled,
                                     const glm::vec2 startPosition) :
    SceneObject(scene)
{
    LineTransform *transform = nullptr;
    m_transformComponent = std::make_unique<LineTransform>();
    transform = static_cast<LineTransform *>(m_transformComponent.get());
    glm::vec4 color(0.0f, 0.5f, 0.0f, 1.0f);
    m_renderableComponent = std::make_unique<LineComponent>(transform, color);
    m_renderableComponent->setEnabled(debugDrawEnabled);
    m_physicsComponent = std::make_unique<RangeSensor>(*m_physicsWorld, transform,
                                                       startPosition, spec.relativeAngle,
                                                       spec.minDistance, spec.maxDistance,
                                                       spec.updateRateSeconds);
    m_rangeSensor = static_cast<RangeSensor *>(m_physicsComponent.get());
}

RangeSensorObject::~RangeSensorObject()
{
}

void RangeSensorObject::setDebugDraw(bool enabled)
{
    m_renderableComponent->setEnabled(enabled);
}

Body2D *RangeSensorObject::getBody() const
{
    return m_rangeSensor->getBody();
}

float *RangeSensorObject::getVoltageLine() const
{
    return m_rangeSensor->getVoltageLine();
}

void RangeSensorObject::onFixedUpdate()
{
}
