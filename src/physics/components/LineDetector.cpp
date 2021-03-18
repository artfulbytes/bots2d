#include "components/LineDetector.h"
#include "components/Body2D.h"
#include "components/Transforms.h"

namespace {
    constexpr float drawRadiusUndetected = 0.001f;
    constexpr float drawRadiusDetected = 10 * drawRadiusUndetected;
}

LineDetector::LineDetector(const PhysicsWorld &world, CircleTransform *transform, const glm::vec2 &startPosition) :
    PhysicsComponent(world),
    m_transform(transform)
{
    /* Represent as tiny body */
    const Body2D::Specification bodySpec { true, false, 0.001f };
    m_body2D = std::make_unique<Body2D>(world, startPosition, 0.0f, drawRadiusUndetected, bodySpec);
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

void LineDetector::onFixedUpdate()
{
    const bool detected = m_userData.contactCount.load() > 0;
    m_detectVoltage = detected ? 3.3f : 0.0f;
    const bool debugDraw = m_transform != nullptr;
    if (debugDraw) {
        m_transform->position = m_body2D->getPosition();
        m_transform->radius = detected ? drawRadiusDetected : drawRadiusUndetected;
    }
}
