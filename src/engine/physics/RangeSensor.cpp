#include "RangeSensor.h"
#include "Body2D.h"
#include "Transforms.h"

#include "box2d/box2d.h"

namespace {
    static const float drawWidth = 0.01f;
}

RangeSensor::RangeSensor(const PhysicsWorld &world, LineTransform *transform, const Body2D &body,
                         const glm::vec2 &position, float angle, float minDistance, float maxDistance) :
    PhysicsComponent(world),
    m_lineTransform(transform),
    m_parentBody(&body),
    m_relativePosition(PhysicsWorld::scalePosition(position)),
    m_relativeAngle(angle),
    m_minDistance(PhysicsWorld::scalePosition(minDistance)),
    m_maxDistance(PhysicsWorld::scaleLength(maxDistance)),
    m_detectedDistance(m_maxDistance)
{
    if (m_lineTransform) {
        m_lineTransform->width = drawWidth;
    }
}

RangeSensor::~RangeSensor()
{
}

void RangeSensor::onFixedUpdate(double stepTime)
{
    const float rayAngleStart = -m_parentBody->getAngle();
    const float rayAngleEnd = m_relativeAngle - m_parentBody->getAngle();
    const glm::vec2 bodyPosition = m_parentBody->getPosition();

    /* Recalculate the casted ray after parent body rotation */
    const float sinAngle = sinf(-rayAngleStart);
    const float cosAngle = cosf(-rayAngleStart);
    const glm::vec2 rayStartPosition = { (m_relativePosition.x * cosAngle - m_relativePosition.y * sinAngle) + bodyPosition.x,
                                    (m_relativePosition.x * sinAngle + m_relativePosition.y * cosAngle) + bodyPosition.y };
    const glm::vec2 rayEndPosition = rayStartPosition + glm::vec2{sinf(rayAngleEnd), cosf(rayAngleEnd)} * m_maxDistance;
    updateDetectedDistance(rayStartPosition, rayEndPosition);
    const glm::vec2 detectedEndPosition = rayStartPosition + glm::vec2{sinf(rayAngleEnd), cosf(rayAngleEnd)} * m_detectedDistance;

    if (m_lineTransform) {
        m_lineTransform->start = { rayStartPosition.x, rayStartPosition.y };
        m_lineTransform->end = { detectedEndPosition.x, detectedEndPosition.y };
    }
    updateVoltage();
}

void RangeSensor::updateDetectedDistance(const glm::vec2 &start, const glm::vec2 &end)
{
    b2RayCastInput rayInput;
    b2RayCastOutput rayOutput;
    rayInput.p1.x = start.x;
    rayInput.p1.y = start.y;
    rayInput.p2.x = end.x;
    rayInput.p2.y = end.y;
    rayInput.maxFraction = 1.0f;

    float closestFraction = rayInput.maxFraction;
    b2Vec2 intersectionNormal(0,0);
    for (b2Body *b = m_world->GetBodyList(); b; b = b->GetNext()) {
        if (b->GetFixtureList() && b->GetFixtureList()[0].IsSensor()) {
            /* Don't detect non-collidable objects */
            continue;
        }
        for (b2Fixture *f = b->GetFixtureList(); f; f = f->GetNext()) {
            if (!f->RayCast(&rayOutput, rayInput, 1)) {
                continue;
            }
            if (rayOutput.fraction < closestFraction) {
                closestFraction = rayOutput.fraction;
                intersectionNormal = rayOutput.normal;
            }
        }
    }

    m_detectedDistance = closestFraction * m_maxDistance;
}

float RangeSensor::getDistance() const
{
    return m_detectedDistance;
}

void RangeSensor::updateVoltage()
{
    m_distanceVoltage = m_detectedDistance / m_maxDistance;
}

float *RangeSensor::getVoltageLine()
{
    return &m_distanceVoltage;
}
