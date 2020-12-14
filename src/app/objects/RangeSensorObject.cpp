#include "RangeSensorObject.h"
#include "Transforms.h"
#include "LineComponent.h"
#include "RangeSensor.h"

#include <glm/glm.hpp>

RangeSensorObject::RangeSensorObject(Scene *scene, const PhysicsWorld &world, const Body2D &parentBody, bool show,
                                     const Specification &unscaledSpec) :
    SceneObject(scene)
{
    LineTransform *transform = nullptr;
    if (show) {
        m_transformComponent = std::make_unique<LineTransform>();
        transform = static_cast<LineTransform *>(m_transformComponent.get());
        glm::vec4 color(0.0f, 0.5f, 0.0f, 1.0f);
        m_renderableComponent = std::make_unique<LineComponent>(transform, color);
    }
    m_physicsComponent = std::make_unique<RangeSensor>(world, transform, parentBody,
                                                       unscaledSpec.relativePosition, unscaledSpec.relativeAngle,
                                                       unscaledSpec.minDistance, unscaledSpec.maxDistance);
    m_rangeSensor = static_cast<RangeSensor *>(m_physicsComponent.get());
}

RangeSensorObject::~RangeSensorObject()
{
}

float *RangeSensorObject::getVoltageLine() const
{
    return m_rangeSensor->getVoltageLine();
}

void RangeSensorObject::onFixedUpdate(double stepTime)
{
}
