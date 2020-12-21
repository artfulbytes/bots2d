#include "sensors/LineDetectorObject.h"
#include "components/LineDetector.h"
#include "components/Transforms.h"
#include "components/CircleComponent.h"
#include "components/Body2D.h"

#include <glm/glm.hpp>

LineDetectorObject::LineDetectorObject(Scene *scene, const PhysicsWorld &world, Body2D &parentBody,
                                       bool show, const glm::vec2 &unscaledRelativePosition) :
    SceneObject(scene)
{
    CircleTransform *circleTransform;
    if (show) {
        m_transformComponent = std::make_unique<CircleTransform>();
        circleTransform = static_cast<CircleTransform *>(m_transformComponent.get());
        glm::vec4 color(1.0f, 0.5f, 0.0f, 1.0f);
        m_renderableComponent = std::make_unique<CircleComponent>(circleTransform, color);
    }
    m_physicsComponent = std::make_unique<LineDetector>(world, circleTransform, parentBody, unscaledRelativePosition);
    m_lineDetector = static_cast<LineDetector *>(m_physicsComponent.get());
}

float *LineDetectorObject::getVoltageLine() const
{
    return m_lineDetector->getVoltageLine();
}

LineDetectorObject::~LineDetectorObject()
{
}

void LineDetectorObject::onFixedUpdate(double stepTime)
{
}
