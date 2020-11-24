#include "RangeSensorObject.h"
#include "Transforms.h"
#include "LineComponent.h"
#include "RangeSensor.h"

#include <glm/glm.hpp>

RangeSensorObject::RangeSensorObject(AppScene &appScene, const PhysicsWorld &world, const Body2D &parentBody, bool show, const Specification &unscaledSpec) :
    AppObject(appScene)
{
    LineTransform *transform = nullptr;
    LineComponent *renderable = nullptr;
    if (show) {
        transform = new LineTransform();
        glm::vec4 color(0.0f, 0.5f, 0.0f, 1.0f);
        renderable = new LineComponent(color);
    }
    m_rangeSensor = new RangeSensor(world, transform, parentBody, unscaledSpec.relativePosition, unscaledSpec.relativeAngle,
                                    unscaledSpec.minDistance, unscaledSpec.maxDistance);
    appScene.getScene()->createObject(transform, renderable, m_rangeSensor, nullptr);
}

float *RangeSensorObject::getVoltageLine() const
{
    return m_rangeSensor->getVoltageLine();
}

RangeSensorObject::~RangeSensorObject()
{
}

void RangeSensorObject::onFixedUpdate(double stepTime)
{
}
