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

void PhysicsWorld::step(float timeStep)
{
    m_world->Step(timeStep, 6, 2);
}
