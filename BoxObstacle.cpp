#include "BoxObstacle.h"
#include "Constants.h"

using namespace constants;

BoxObstacle::BoxObstacle(b2World* world, float x, float y, float width, float mass)
{
    assert(minWidthObject < width && width < maxWidthObject);
    const float scaledWidth = width * lengthScaleFactor;
    const float scaledMass = mass * massScaleFactor;
    const float normalForce = gravitationConstant * mass * forceScaleFactor;
    const float area = scaledWidth * scaledWidth;
    const float density = scaledMass / area;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(x * lengthScaleFactor, y * lengthScaleFactor);
    m_body = world->CreateBody(&bodyDef);
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(scaledWidth / 2, scaledWidth / 2);
    m_body->CreateFixture(&polygonShape, density);

    /* Add friction */
    b2BodyDef frictionBodyDef;
    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = true;
    m_frictionBody = world->CreateBody(&frictionBodyDef);
    b2FrictionJointDef jointDef;
    jointDef.bodyA = m_frictionBody;
    jointDef.bodyB = m_body;
    jointDef.maxForce = normalForce * frictionCoefficient;
    /* Setting maxTorque to same as maxForce might not be realistic... (but works for now) */
    jointDef.maxTorque = normalForce * frictionCoefficient;
    m_frictionJoint = world->CreateJoint(&jointDef);
}

/* TODO: Add destructor */
