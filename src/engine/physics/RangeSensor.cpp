#include "RangeSensor.h"
#include "Body2D.h"
#include "Transforms.h"

#include "box2d/box2d.h"

namespace {
    static const float debugDrawWidth = 0.002f;
}

RangeSensor::RangeSensor(const PhysicsWorld &world, LineTransform &transform, const Body2D &body,
                         const Vec2 &position, float angle, float minDistance, float maxDistance) :
    PhysicsComponent(world),
    m_lineTransform(&transform),
    m_parentBody(&body),
    m_relativePosition(PhysicsWorld::scalePosition(position)),
    m_relativeAngle(angle),
    m_minDistance(PhysicsWorld::scalePosition(minDistance)),
    m_maxDistance(PhysicsWorld::scaleLength(maxDistance)),
    m_detectedDistance(m_maxDistance)
{
    m_lineTransform->width = PhysicsWorld::scalePosition(debugDrawWidth);
}

void RangeSensor::onFixedUpdate(double stepTime)
{
    const float rayAngleStart = -m_parentBody->getAngle();
    const float rayAngleEnd = m_relativeAngle - m_parentBody->getAngle();
    const Vec2 bodyPosition = m_parentBody->getPosition();

    /* Recalculate the casted ray after parent body rotation */
    const float sinAngle = sinf(-rayAngleStart);
    const float cosAngle = cosf(-rayAngleStart);
    const Vec2 rayStartPosition = { (m_relativePosition.x * cosAngle - m_relativePosition.y * sinAngle) + bodyPosition.x,
                                    (m_relativePosition.x * sinAngle + m_relativePosition.y * cosAngle) + bodyPosition.y };
    const Vec2 rayEndPosition = rayStartPosition + Vec2(sinf(rayAngleEnd), cosf(rayAngleEnd)) * m_maxDistance;
    updateDetectedDistance(rayStartPosition, rayEndPosition);
    const Vec2 detectedEndPosition = rayStartPosition + Vec2(sinf(rayAngleEnd), cosf(rayAngleEnd)) * m_detectedDistance;

    m_lineTransform->start = { rayStartPosition.x, rayStartPosition.y };
    m_lineTransform->end = { detectedEndPosition.x, detectedEndPosition.y };
    updateVoltage();
}

void RangeSensor::updateDetectedDistance(const Vec2 &start, const Vec2 &end)
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

const float *RangeSensor::getVoltageLine() const
{
    return &m_distanceVoltage;
}
