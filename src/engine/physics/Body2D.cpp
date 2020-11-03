#include "Body2D.h"
#include "Transforms.h"
#include "Constants.h"
#include "PhysicsWorld.h"

#include "box2d/box2d.h"
#include <iostream>

using namespace constants;

/* TODO: Are these cleaned up properly? */
void Body2D::QuadTransformTranslator::translate()
{
    const b2Vec2 position = m_body->GetPosition();
    m_transform->position.x = position.x;
    m_transform->position.y = position.y;
    m_transform->rotation = m_body->GetAngle();
}

void Body2D::CircleTransformTranslator::translate()
{
}

void Body2D::addTopViewFriction(float normalForce) {
    b2BodyDef frictionBodyDef;
    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = true;
    m_frictionBody = m_world->CreateBody(&frictionBodyDef);
    b2FrictionJointDef jointDef;
    jointDef.bodyA = m_frictionBody;
    jointDef.bodyB = m_body;
    jointDef.maxForce = normalForce * frictionCoefficient;
    /* Setting maxTorque to same as maxForce might not be realistic... (but works for now) */
    jointDef.maxTorque = normalForce * frictionCoefficient;
    m_frictionJoint = m_world->CreateJoint(&jointDef);
}

Body2D::Body2D(const PhysicsWorld &world, QuadTransform &transform, bool dynamic, float mass) :
    PhysicsComponent(world)
{
    /* TODO: Add common place for assert function (statics in PhysicsWorld?) */
    assert(minWidthObject <= transform.size.x && transform.size.x <= maxWidthObject);
    assert(minWidthObject <= transform.size.y && transform.size.y <= maxWidthObject);
    /* TODO: Move scaling somewhere else (statics in transforms or in Physicsworld?) */
    /* TODO: LengthScaleFactor should be physics namespace */
    const float scaledWidth = transform.size.x * lengthScaleFactor;
    const float scaledHeight = transform.size.y * lengthScaleFactor;
    transform.size.x = scaledWidth;
    transform.size.y = scaledHeight;
    const float posX = transform.position.x * lengthScaleFactor;
    const float posY = transform.position.y * lengthScaleFactor;
    b2BodyDef bodyDef;
    bodyDef.type = dynamic ? b2_dynamicBody : b2_staticBody;
    bodyDef.position = b2Vec2(posX + scaledWidth / 2,
                              posY + scaledHeight / 2);
    bodyDef.angle = transform.rotation;
    m_body = m_world->CreateBody(&bodyDef);
    m_translator = new QuadTransformTranslator(transform, *m_body);
    const float scaledMass = mass * massScaleFactor;
    const float normalForce = gravitationConstant * mass * forceScaleFactor;
    const float area = scaledWidth * scaledHeight;
    const float density = scaledMass / area;

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(scaledWidth / 2, scaledHeight / 2);
    m_body->CreateFixture(&polygonShape, density);

    if (world.getGravityType() == PhysicsWorld::Gravity::TopView) {
        addTopViewFriction(normalForce);
    }
}

void Body2D::update()
{
    if (nullptr == m_translator) {
        return;
    }
    m_translator->translate();
}

