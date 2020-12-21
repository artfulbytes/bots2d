#include "components/LineDetector.h"
#include "components/Body2D.h"
#include "components/Transforms.h"

#include <box2d/box2d.h>

namespace {
    constexpr float radius = 0.005f;
}

LineDetector::LineDetector(const PhysicsWorld &world, CircleTransform *transform, Body2D &parentBody, const glm::vec2 &relativePosition) :
    PhysicsComponent(world),
    m_transform(transform)
{
    const auto scaledRelativePosition = PhysicsWorld::scalePosition(relativePosition);
    if (m_transform) {
        m_transform->position = { scaledRelativePosition.x, scaledRelativePosition.y };
        m_transform->radius = radius;
    }
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.angle = 0.0f;
    m_body = m_world->CreateBody(&bodyDef);
    m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&m_userData);

    /* Fixture (tiny circle) */
    b2CircleShape circleShape;
    circleShape.m_radius = radius;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.isSensor = true;
    fixtureDef.density = 0.01f;
    m_body->CreateFixture(&fixtureDef);

    b2WeldJointDef jointDef;
    jointDef.bodyA = parentBody.m_body;
    jointDef.localAnchorA.Set(scaledRelativePosition.x,
                              scaledRelativePosition.y);
    jointDef.localAnchorB.SetZero();
    jointDef.bodyB = m_body;
    m_world->CreateJoint(&jointDef);
}

LineDetector::~LineDetector()
{
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
        const auto position = m_body->GetPosition();
        m_transform->position = { position.x, position.y };
        m_transform->radius = detected ? 50.0f * radius : radius;
    }
}
