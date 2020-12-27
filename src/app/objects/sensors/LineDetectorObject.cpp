#include "sensors/LineDetectorObject.h"
#include "components/LineDetector.h"
#include "components/Transforms.h"
#include "components/CircleComponent.h"
#include "components/Body2D.h"

#include <glm/glm.hpp>

LineDetectorObject::LineDetectorObject(Scene *scene, bool debugDrawEnabled,
                                       const glm::vec2 &startPosition) :
    SceneObject(scene)
{
    CircleTransform *circleTransform;
    m_transformComponent = std::make_unique<CircleTransform>();
    circleTransform = static_cast<CircleTransform *>(m_transformComponent.get());
    glm::vec4 color(1.0f, 0.5f, 0.0f, 1.0f);
    m_renderableComponent = std::make_unique<CircleComponent>(circleTransform, color);
    m_renderableComponent->setEnabled(debugDrawEnabled);
    m_physicsComponent = std::make_unique<LineDetector>(*m_physicsWorld, circleTransform, startPosition);
    m_lineDetector = static_cast<LineDetector *>(m_physicsComponent.get());
}

LineDetectorObject::~LineDetectorObject()
{
}

Body2D *LineDetectorObject::getBody() const
{
    return m_lineDetector->getBody();
}

float *LineDetectorObject::getVoltageLine() const
{
    return m_lineDetector->getVoltageLine();
}

void LineDetectorObject::setDebugDraw(bool enabled)
{
    m_renderableComponent->setEnabled(enabled);
}

void LineDetectorObject::onFixedUpdate(double stepTime)
{
}
