#include "LineDetector.h"
#include "UserData.h"
#include <iostream>
#include <../external/Box2D/include/box2d/b2_body.h>

#define DEBUG_DRAW

LineDetector::LineDetector(b2World* world, b2Body* ownerBody, b2Vec2 position) :
    m_world(world),
    m_ownerBody(ownerBody),
    m_position(position)
{
    /* Body */
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    /* Fixture (tiny circle) */
    b2CircleShape circleShape;
    circleShape.m_radius = 0.01f;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.isSensor = true;
    fixtureDef.density = 0.01f;
    m_body->CreateFixture(&fixtureDef);

    /* Joint */
    b2WeldJointDef jointDef;
    jointDef.bodyA = m_ownerBody;
    jointDef.localAnchorA.Set(m_position.x, m_position.y);
    jointDef.localAnchorB.SetZero();
    jointDef.bodyB = m_body;
    m_joint = world->CreateJoint(&jointDef);

    /* User data */
    UserData* userData = new UserData();
    userData->bodyType = BodyType::LineDetector;
    m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(userData);
}

bool LineDetector::detected()
{
    /* TODO: Before we did static cast here and no &... */
    const bool detected = reinterpret_cast<UserData*>(&m_body->GetUserData())->contactCount > 0;

#ifdef DEBUG_DRAW
    if (detected) {
        //g_debugDraw.DrawCircle(m_body->GetPosition(), 0.4f, b2Color(0.7f, 0.4f, 0.4f));
    }
#endif //DEBUG_DRAW
    return detected;
}

/* TODO: Add destructor (freeing body, joint, userdata) */
