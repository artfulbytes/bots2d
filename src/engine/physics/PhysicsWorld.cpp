#include "PhysicsWorld.h"
#include "Constants.h"

#include "box2d/box2d.h"
#include <cassert>

// TODO: Fix inline variables warning
// TODO: Should they be inline constexpr?
// TODO: Namespace?

/*
* Box2D uses metric units and recommends sizing objects
* between 0.1-10 m. In this simulation tool we want
* sizes as small as 0.01 m. Therefore, introduce
* a scale factor of 10, which converts 1 meter in Box2D
* 10 cm, giving us a range between 0.01-1 m for our objects.
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

inline constexpr float gravitationConstant { 9.82 };

void PhysicsWorld::assertDimensions(float length)
{
    assert(minWidthObject <= length && length <= maxWidthObject);
}

float PhysicsWorld::scaleLength(float length)
{
    assertDimensions(length);
    return length * lengthScaleFactor;
}

float PhysicsWorld::scalePosition(float position)
{
    /* No dimension limitation for position x or y */
    return position * lengthScaleFactor;
}

float PhysicsWorld::scaleSpeed(float speed)
{
    return speed * speedScaleFactor;
}

float PhysicsWorld::scaleAcceleration(float acceleration)
{
    return acceleration * accelerationScaleFactor;
}

float PhysicsWorld::scaleMass(float mass)
{
    return mass * massScaleFactor;
}

float PhysicsWorld::scaleForce(float force)
{
    return force * forceScaleFactor;
}

float PhysicsWorld::normalForce(float mass)
{
    return mass * gravitationConstant * forceScaleFactor;
}

PhysicsWorld::PhysicsWorld(Gravity gravity)
{
    switch (gravity)
    {
    case Gravity::SideView:
        /* The typical Box2D physics */
        m_world = new b2World(b2Vec2(0, -gravitationConstant));
        break;
    case Gravity::TopView:
        /* Box2D is not designed for top view physics, but we
           can work around this by setting gravity to 0 */
        m_world = new b2World(b2Vec2(0, 0));
        break;
    case Gravity::Custom:
        /* Should use the other constructor */
        assert(false);
    }
    m_gravityType = gravity;
}

PhysicsWorld::PhysicsWorld(float gravityX, float gravityY) :
    m_world(new b2World(b2Vec2(gravityX, gravityY))),
    m_gravityType(Gravity::Custom)
{
    //s_physicsData->world->SetContactListener(new ContactListener());
}

PhysicsWorld::~PhysicsWorld()
{
    /* TODO: Implement smart ptrs */
    /* TODO: Delete b2 objects? */
    delete m_world;
}

void PhysicsWorld::step(double stepTime)
{
    /* The iteration values 6 and 2 are recommended value taken from elsewhere.
       They matter when calculating collision. */
    m_world->Step(stepTime, 6, 2);
}
