#include "LineDetectorObject.h"
#include "Transforms.h"
#include "CircleComponent.h"
#include "LineDetector.h"
#include "Body2D.h"

#include <glm/glm.hpp>


LineDetectorObject::LineDetectorObject(AppScene &appScene, const PhysicsWorld &world, Body2D &parentBody,
                                       bool show, const Vec2 &unscaledRelativePosition) :
    AppObject(appScene)
{
    CircleTransform *transform = nullptr;
    CircleComponent *renderable = nullptr;
    if (show) {
        transform = new CircleTransform();
        glm::vec4 color(1.0f, 0.5f, 0.0f, 1.0f);
        renderable = new CircleComponent(color);
    }
    m_lineDetector = new LineDetector(world, transform, parentBody, unscaledRelativePosition);
    appScene.getScene()->createObject(transform, renderable, m_lineDetector, nullptr);
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
