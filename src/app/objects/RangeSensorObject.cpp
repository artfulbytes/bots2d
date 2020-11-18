#include "RangeSensorObject.h"
#include "Transforms.h"
#include "LineComponent.h"
#include "RangeSensor.h"

#include <glm/glm.hpp>

RangeSensorObject::RangeSensorObject(AppScene &appScene, const PhysicsWorld &world, const Body2D &parentBody, const Specification &unscaledSpec) :
    AppObject(appScene)
{
    LineTransform *transform = new LineTransform();
    m_rangeSensor = new RangeSensor(world, *transform, parentBody, unscaledSpec.relativePosition, unscaledSpec.relativeAngle,
                                    unscaledSpec.minDistance, unscaledSpec.maxDistance);
    glm::vec4 color(0.0f, 0.5f, 0.0f, 1.0f);
    LineComponent *renderable = new LineComponent(color);
    appScene.getScene()->createObject(transform, renderable, m_rangeSensor, nullptr);
}

const float *RangeSensorObject::getVoltageLine() const
{
    return m_rangeSensor->getVoltageLine();
}

RangeSensorObject::~RangeSensorObject()
{
}

void RangeSensorObject::onFixedUpdate(double stepTime)
{
}
