#include "RangeSensor.h"
#include "draw.h"
#include <iostream>

#define DEBUG_DRAW

RangeSensor::RangeSensor(b2World* world, b2Body* body, b2Vec2 position, float angle, float minDistance, float maxDistance) :
    m_world(world),
    m_body(body),
    m_position(position),
    m_angle(angle),
    m_minDistance(minDistance),
    m_maxDistance(maxDistance)
{
}

void RangeSensor::update(void)
{
    const float rayAngle = m_angle - m_body->GetAngle();
    const b2Vec2 bodyPosition = m_body->GetPosition();

    /* Calculate new sensor position (start of ray) after body rotation */
    const float sinAngle = sinf(-rayAngle);
    const float cosAngle = cosf(-rayAngle);
    m_rayStartPosition = m_position;
    m_rayStartPosition.x = m_position.x * cosAngle - m_position.y * sinAngle;
    m_rayStartPosition.y = m_position.x * sinAngle + m_position.y * cosAngle;
    m_rayStartPosition.x += bodyPosition.x;
    m_rayStartPosition.y += bodyPosition.y;

    /* The sensor casts a straight ray, calculate the ray's end position */
    m_rayEndPosition = m_rayStartPosition + m_maxDistance * b2Vec2(sinf(rayAngle), cosf(rayAngle));
}

float RangeSensor::getDistance()
{
    b2RayCastInput rayInput;
    b2RayCastOutput rayOutput;
    rayInput.p1 = m_rayStartPosition;
    rayInput.p2 = m_rayEndPosition;
    rayInput.maxFraction = 1.0f;

    float closestFraction = rayInput.maxFraction;
    b2Vec2 intersectionNormal(0,0);
    for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext()) {
        if (b->GetFixtureList() && b->GetFixtureList()[0].IsSensor()) {
            /* Don't detect non-collidable objects */
            continue;
        }
        for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
            if (!f->RayCast(&rayOutput, rayInput, 1)) {
                continue;
            }
            if (rayOutput.fraction < closestFraction) {
                closestFraction = rayOutput.fraction;
                intersectionNormal = rayOutput.normal;
            }
        }
    }

#ifdef DEBUG_DRAW
    const b2Vec2 intersectionPoint = m_rayStartPosition + closestFraction * (m_rayEndPosition - m_rayStartPosition);
    b2Vec2 rayPoints[2];
    rayPoints[0] = m_rayStartPosition;
    rayPoints[1] = intersectionPoint;
    // TODO: Show debug draw???
    g_debugDraw.DrawPolygon(rayPoints, 2,  b2Color(0.4f, 0.9f, 0.4f));
    g_debugDraw.DrawCircle(rayPoints[1], 0.1f, b2Color(0.7f, 0.4f, 0.4f));
#endif //DEBUG_DRAW

    return closestFraction * m_maxDistance;
}
