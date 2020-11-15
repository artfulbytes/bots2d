#include "PhysicsWorld.h"
#include "Constants.h"

#include "box2d/box2d.h"
#include <cassert>

PhysicsWorld::PhysicsWorld(Gravity gravity)
{
    switch (gravity)
    {
    case Gravity::SideView:
        /* The typical Box2D physics */
        m_world = new b2World(b2Vec2(0, -constants::gravitationConstant));
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
