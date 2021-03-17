#include "PhysicsWorld.h"
#include "ContactListener.h"

#include "box2d/box2d.h"
#include <cassert>

namespace {
constexpr float maxWidthObject { 1.0f };
constexpr float minWidthObject { 0.01f };
constexpr float physScaleFactor { 10.0f };
constexpr float lengthScaleFactor { physScaleFactor };
constexpr float speedScaleFactor { lengthScaleFactor };
constexpr float accelerationScaleFactor { speedScaleFactor };
constexpr float massScaleFactor { physScaleFactor * physScaleFactor * physScaleFactor };
/* F = m*a so F_scaled = forceFactor * F = (massFactor * m) * (accFactor * a) */
const float forceScaleFactor { massScaleFactor * accelerationScaleFactor };
constexpr float gravitationConstant { 9.82f };
}

void PhysicsWorld::assertDimensions(float unscaledLength)
{
    assert(minWidthObject <= unscaledLength && unscaledLength <= maxWidthObject);
}

float PhysicsWorld::scaleLength(float unscaledLength)
{
    assertDimensions(unscaledLength);
    return unscaledLength * lengthScaleFactor;
}

float PhysicsWorld::scaleLengthNoAssert(float unscaledLength)
{
    assert(unscaledLength >= 0.0f);
    return unscaledLength * lengthScaleFactor;
}

float PhysicsWorld::unscaleLength(float scaledLength)
{
    const float unscaledLength = scaledLength / lengthScaleFactor;
    assertDimensions(unscaledLength);
    return unscaledLength;
}

float PhysicsWorld::scaleRadius(float unscaledRadius)
{
    return scaleLength(unscaledRadius * 2.0f) / 2.0f;
}

float PhysicsWorld::scalePosition(float unscaledPosition)
{
    /* No dimension limitation for position x or y */
    return unscaledPosition * lengthScaleFactor;
}

glm::vec2 PhysicsWorld::scalePosition(const glm::vec2 &unscaledPosition)
{
    return { scalePosition(unscaledPosition.x), scalePosition(unscaledPosition.y) };
}

float PhysicsWorld::unscalePosition(const float scaledPosition)
{
    return scaledPosition / lengthScaleFactor;
}

glm::vec2 PhysicsWorld::unscalePosition(const glm::vec2 &scaledPosition)
{
    return { unscalePosition(scaledPosition.x), unscalePosition(scaledPosition.y) };
}

float PhysicsWorld::scaleSpeed(float unscaledSpeed)
{
    return unscaledSpeed * speedScaleFactor;
}

float PhysicsWorld::unscaleSpeed(float scaledSpeed)
{
    return scaledSpeed / speedScaleFactor;
}

float PhysicsWorld::scaleAcceleration(float unscaledAcceleration)
{
    return unscaledAcceleration * accelerationScaleFactor;
}

float PhysicsWorld::scaleMass(float unscaledMass)
{
    assert(unscaledMass >= 0.0f);
    return unscaledMass * massScaleFactor;
}

float PhysicsWorld::unscaleMass(float scaledMass)
{
    assert(scaledMass >= 0.0f);
    return scaledMass / massScaleFactor;
}

float PhysicsWorld::scaleForce(float unscaledForce)
{
    return unscaledForce * forceScaleFactor;
}

float PhysicsWorld::unscaleForce(float scaledForce)
{
    return scaledForce / forceScaleFactor;
}

float PhysicsWorld::normalForce(float unscaledMass)
{
    assert(unscaledMass >= 0.0f);
    return unscaledMass * gravitationConstant * forceScaleFactor;
}

void PhysicsWorld::init()
{
    m_contactListener = std::make_unique<ContactListener>();
    m_world->SetContactListener(m_contactListener.get());
}

PhysicsWorld::PhysicsWorld(Gravity gravity) :
    m_gravityType(gravity)
{
    switch (gravity)
    {
    case Gravity::SideView:
        /* The typical Box2D physics */
        m_world = std::make_unique<b2World>(b2Vec2(0, -gravitationConstant));
        break;
    case Gravity::TopView:
        /* Box2D is not designed for top view physics, but we
           can work around this by setting gravity to 0 */
        m_world = std::make_unique<b2World>(b2Vec2(0, 0));
        break;
    case Gravity::Custom:
        /* If you end up here, you have used the wrong constructor, use the other one */
        assert(false);
    }
    m_gravityType = gravity;
    init();
}

PhysicsWorld::PhysicsWorld(float gravityX, float gravityY) :
    m_world(std::make_unique<b2World>(b2Vec2(gravityX, gravityY))),
    m_gravityType(Gravity::Custom)
{
    init();
}

PhysicsWorld::~PhysicsWorld()
{
}

void PhysicsWorld::step(float stepTime)
{
    /* The iteration values 6 and 2 are recommended value taken from elsewhere.
       They matter when calculating collision. */
    m_world->Step(stepTime, 6, 2);
}
