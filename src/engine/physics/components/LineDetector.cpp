#include "components/LineDetector.h"
#include "components/Body2D.h"
#include "components/Transforms.h"

namespace {
    constexpr float radius = 0.005f;
}

LineDetector::LineDetector(const PhysicsWorld &world, CircleTransform *transform, const glm::vec2 &unscaledRelativePos) :
    PhysicsComponent(world),
    m_transform(transform)
{
    /* Represent as tiny body */
    const Body2D::Specification bodySpec { true, false, 0.001f };
    m_body2D = std::make_unique<Body2D>(world, unscaledRelativePos, 0.0f, radius, bodySpec);
    m_body2D->setUserData(&m_userData);
}

LineDetector::~LineDetector()
{
}

Body2D *LineDetector::getBody() const
{
    return m_body2D.get();
}

float *LineDetector::getVoltageLine()
{
    return &m_detectVoltage;
}

void LineDetector::onFixedUpdate(double stepTime)
{
    const bool detected = m_userData.contactCount > 0;
    m_detectVoltage = detected ? 3.3f : 0.0f;
    if (m_transform) {
        m_transform->position = m_body2D->getPosition();
        m_transform->radius = detected ? 50.0f * radius : radius;
    }
}
