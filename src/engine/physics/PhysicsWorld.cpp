#include "PhysicsWorld.h"
#include "ContactListener.h"

#include "box2d/box2d.h"
#include <cassert>

/* Box2D uses metric units and documentation recommends sizing objects between 0.1-10 m.
 * We want sizes as small as 0.01 m, so introduce a scale factor of 10 to give us a working
 * range of 0.01-1 m instead.
 *
 * All scaling should be handled inside the physics components and be hidden from the
 * other classes. That is, physics components work in the range 0.1-10 m while the
 * other classes work in 0.01-1 m.
 */
inline constexpr float maxWidthObject { 1.0f };
inline constexpr float minWidthObject { 0.01f };
inline constexpr float physScaleFactor { 10.0f };
inline constexpr float lengthScaleFactor { physScaleFactor };
inline constexpr float speedScaleFactor { lengthScaleFactor };
inline constexpr float accelerationScaleFactor { speedScaleFactor };
inline constexpr float massScaleFactor { physScaleFactor * physScaleFactor * physScaleFactor };
/* F = m*a so F_scaled = forceFactor * F = (massFactor * m) * (accFactor * a) */
inline constexpr float forceScaleFactor { massScaleFactor * accelerationScaleFactor };

inline constexpr float gravitationConstant { 9.82f };

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
    return unscaledMass * massScaleFactor;
}

float PhysicsWorld::unscaleMass(float scaledMass)
{
    return scaledMass / massScaleFactor;
}

float PhysicsWorld::scaleForce(float unscaledForce)
{
    return unscaledForce * forceScaleFactor;
}

float PhysicsWorld::normalForce(float unscaledMass)
{
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

void PhysicsWorld::step(double stepTime)
{
    /* The iteration values 6 and 2 are recommended value taken from elsewhere.
       They matter when calculating collision. */
    m_world->Step(stepTime, 6, 2);
}
