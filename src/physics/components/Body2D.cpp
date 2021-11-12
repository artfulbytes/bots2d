#include "components/Body2D.h"
#include "components/Transforms.h"
#include "PhysicsWorld.h"
#include "Body2DUserData.h"
#include "QuadCoords.h"

#include <box2d/box2d.h>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>

namespace {
class RectTransformTranslator : public PhysicsToTransformTranslator
{
public:
    RectTransformTranslator(RectTransform *transform, b2Body *body) :
        m_transform(transform), m_body(body) {}
    void translate()
    {
        const b2Vec2 position = m_body->GetPosition();
        m_transform->position = PhysicsWorld::unscalePosition({ position.x, position.y });
        m_transform->rotation = m_body->GetAngle();
    }

private:
    RectTransform *m_transform = nullptr;
    b2Body *m_body = nullptr;
};

class CircleTransformTranslator : public PhysicsToTransformTranslator
{
public:
    CircleTransformTranslator(CircleTransform *transform, b2Body *body) :
        m_transform(transform), m_body(body) {}
    void translate()
    {
        const b2Vec2 position = m_body->GetPosition();
        m_transform->position = PhysicsWorld::unscalePosition({ position.x, position.y });
        m_transform->rotation = m_body->GetAngle();
    }

private:
    CircleTransform *m_transform = nullptr;
    b2Body *m_body = nullptr;
};

constexpr int totalVertexCount = 180;
constexpr float anglePerVertex = 2 * glm::pi<float>() / totalVertexCount;
constexpr int trapezoidVertexCount = 4;
}

Body2D::Body2D(const PhysicsWorld &world, RectTransform *transform, const Body2D::Specification &spec) :
    PhysicsComponent(world)
{
    assert(transform != nullptr);
    const float scaledWidth = PhysicsWorld::scaleLength(transform->size.x);
    const float scaledLength = PhysicsWorld::scaleLength(transform->size.y);
    const glm::vec2 scaledPosition = PhysicsWorld::scalePosition(transform->position);
    const float scaledMass = PhysicsWorld::scaleMass(spec.mass);
    const float normalForce = PhysicsWorld::normalForce(spec.mass);
    const float scaledArea = scaledWidth * scaledLength;
    const float scaledDensity = scaledMass / scaledArea;

    b2BodyDef bodyDef;
    bodyDef.type = spec.dynamic ? b2_dynamicBody : b2_staticBody;
    bodyDef.position = b2Vec2(scaledPosition.x, scaledPosition.y);
    bodyDef.angle = transform->rotation;
    m_body = m_world->CreateBody(&bodyDef);

    b2FixtureDef fixtureDef;
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(scaledWidth / 2, scaledLength / 2);
    fixtureDef.shape = &polygonShape;
    fixtureDef.isSensor = !spec.collision;
    fixtureDef.density = scaledDensity;
    m_body->CreateFixture(&fixtureDef);
    setAngularDamping(spec.angularDamping);

    m_translator = std::make_unique<RectTransformTranslator>(transform, m_body);

    if (world.getGravityType() == PhysicsWorld::Gravity::TopView) {
        addTopViewFriction(normalForce, spec.frictionCoefficient);
    }
}

Body2D::Body2D(const PhysicsWorld &world, const glm::vec2 &startPosition, float rotation,
               float radius, const Body2D::Specification &spec) :
    PhysicsComponent(world)
{
    const float scaledRadius = spec.collision ? PhysicsWorld::scaleRadius(radius)
                                              : PhysicsWorld::scaleLengthNoAssert(radius);
    const float scaledMass = PhysicsWorld::scaleMass(spec.mass);
    const float normalForce = PhysicsWorld::normalForce(spec.mass);
    const float scaledArea = glm::pi<float>() * scaledRadius * scaledRadius;
    const float scaledDensity = scaledMass / scaledArea;
    const auto scaledPosition = PhysicsWorld::scalePosition(startPosition);

    b2BodyDef bodyDef;
    bodyDef.type = spec.dynamic ? b2_dynamicBody : b2_staticBody;
    bodyDef.position = b2Vec2(scaledPosition.x, scaledPosition.y);
    bodyDef.angle = rotation;
    m_body = m_world->CreateBody(&bodyDef);

    b2FixtureDef fixtureDef;
    b2CircleShape circleShape;
    circleShape.m_radius = scaledRadius;
    fixtureDef.shape = &circleShape;
    fixtureDef.isSensor = !spec.collision;
    fixtureDef.density = scaledDensity;
    m_body->CreateFixture(&fixtureDef);
    setAngularDamping(spec.angularDamping);

    if (world.getGravityType() == PhysicsWorld::Gravity::TopView) {
        addTopViewFriction(normalForce, spec.frictionCoefficient);
    }
}

Body2D::Body2D(const PhysicsWorld &world, CircleTransform *transform, const Body2D::Specification &spec) :
    Body2D(world, transform->position, transform->rotation, transform->radius, spec)
{
    m_translator = std::make_unique<CircleTransformTranslator>(transform, m_body);
}

Body2D::Body2D(const PhysicsWorld &world, HollowCircleTransform *transform, const Body2D::Specification &spec) :
    PhysicsComponent(world)
{
    assert(transform != nullptr);
    /* Only support static for now */
    assert(!spec.dynamic);
    assert(world.getGravityType() == PhysicsWorld::Gravity::TopView);
    assert(transform->outerRadius > transform->innerRadius);
    const float scaledInnerRadius = PhysicsWorld::scaleLength(transform->innerRadius);
    const float scaledOuterRadius = PhysicsWorld::scaleLength(transform->outerRadius);
    const glm::vec2 scaledPosition = PhysicsWorld::scalePosition(transform->position);

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(scaledPosition.x, scaledPosition.y);

    m_body = m_world->CreateBody(&bodyDef);
    b2PolygonShape shape;
    b2Vec2 trapezoidVertices[trapezoidVertexCount];
    float angle=0;
    for (int cornerIdx=0; cornerIdx < totalVertexCount; cornerIdx++) {
        angle = cornerIdx * anglePerVertex;
        trapezoidVertices[0].Set(scaledInnerRadius * cosf(angle), scaledInnerRadius * sinf(angle));
        trapezoidVertices[1].Set(scaledOuterRadius * cosf(angle), scaledOuterRadius * sinf(angle));
        angle += anglePerVertex;
        trapezoidVertices[2].Set(scaledOuterRadius * cosf(angle), scaledOuterRadius * sinf(angle));
        trapezoidVertices[3].Set(scaledInnerRadius * cosf(angle), scaledInnerRadius * sinf(angle));
        shape.Set(trapezoidVertices, trapezoidVertexCount);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.isSensor = !spec.collision;
        m_body->CreateFixture(&fixtureDef);
    }
    setAngularDamping(spec.angularDamping);

    /* Only static so no translation needed for now */
    m_translator = nullptr;
}

Body2D::Body2D(const PhysicsWorld &world, QuadTransform *transform, const Body2D::Specification &spec) :
    PhysicsComponent(world)
{
    assert(transform != nullptr);
    (void)spec;
    b2BodyDef bodyDef;
    /* Quad body doesn't support dynamic or collision yet */
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(0, 0);

    m_body = m_world->CreateBody(&bodyDef);
    b2PolygonShape shape;
    b2Vec2 quadVertices[4];
    QuadCoords scaledQuadCoords = {
        PhysicsWorld::scalePosition(transform->quadCoords.BottomLeft),
        PhysicsWorld::scalePosition(transform->quadCoords.BottomRight),
        PhysicsWorld::scalePosition(transform->quadCoords.TopRight),
        PhysicsWorld::scalePosition(transform->quadCoords.TopLeft)
    };
    quadVertices[0].Set(scaledQuadCoords.BottomLeft.x, scaledQuadCoords.BottomLeft.y);
    quadVertices[1].Set(scaledQuadCoords.BottomRight.x, scaledQuadCoords.BottomRight.y);
    quadVertices[2].Set(scaledQuadCoords.TopRight.x, scaledQuadCoords.TopRight.y);
    quadVertices[3].Set(scaledQuadCoords.TopLeft.x, scaledQuadCoords.TopLeft.y);
    shape.Set(quadVertices, 4);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    /* Quad body doesn't support dynamic or collision yet */
    fixtureDef.isSensor = true;
    m_body->CreateFixture(&fixtureDef);

    /* Only static so no translation needed for now */
    m_translator = nullptr;
}


Body2D::~Body2D()
{
    m_world->DestroyBody(m_body);
    if (m_frictionBody != nullptr) {
        m_world->DestroyBody(m_frictionBody);
    }
}

void Body2D::onFixedUpdate()
{
    if (nullptr == m_translator) {
        return;
    }
    m_translator->translate();
}

void Body2D::setUserData(Body2DUserData *userData) {
    m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(userData);
}

void Body2D::attachBodyWithRevoluteJoint(const glm::vec2 &attachPos, const Body2D *body)
{
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = m_body;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    /* Center of the body to be attached */
    jointDef.localAnchorB.SetZero();
    jointDef.bodyB = body->m_body;
    jointDef.localAnchorA.Set(PhysicsWorld::scalePosition(attachPos.x),
                              PhysicsWorld::scalePosition(attachPos.y));
    /* No need to explicitly delete joint, it's deleted when the attached body is deleted */
    m_world->CreateJoint(&jointDef);
}

void Body2D::attachBodyWithWeldJoint(const glm::vec2 &attachPos, const Body2D *body)
{
    b2WeldJointDef jointDef;
    jointDef.bodyA = m_body;
    jointDef.localAnchorA.Set(PhysicsWorld::scalePosition(attachPos.x),
                              PhysicsWorld::scalePosition(attachPos.y));
    jointDef.localAnchorB.SetZero();
    jointDef.bodyB = body->m_body;
    /* No need to explicitly delete joint, it's deleted when the attached body is deleted */
    m_world->CreateJoint(&jointDef);
}

void Body2D::setPositionAndRotation(const glm::vec2 &position, float rotation)
{
    const b2Vec2 scaledPosition = { PhysicsWorld::scalePosition(position.x), PhysicsWorld::scalePosition(position.y) };
    m_body->SetTransform(scaledPosition, rotation);
}

void Body2D::setForce(const glm::vec2 &vec, float magnitude)
{
    const float scaledMagnitude = PhysicsWorld::scaleForce(magnitude);
    m_body->ApplyForce(scaledMagnitude * b2Vec2(vec.x, vec.y), m_body->GetWorldCenter(), true);
}

void Body2D::setLinearImpulse(const glm::vec2 &vec)
{
    b2Vec2 box2DVec(PhysicsWorld::scaleForce(vec.x), PhysicsWorld::scaleForce(vec.y));
    m_body->ApplyLinearImpulse(b2Vec2(vec.x, vec.y), m_body->GetWorldCenter(), true);
}

glm::vec2 Body2D::getPosition() const
{
    auto position = m_body->GetPosition();
    return { PhysicsWorld::unscalePosition(position.x), PhysicsWorld::unscalePosition(position.y) };
}

float Body2D::getRotation() const
{
    return m_body->GetAngle();
}

float Body2D::getForwardSpeed() const
{
    b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0,1));
    const float scaledSpeed = b2Dot(currentForwardNormal, m_body->GetLinearVelocity());
    return PhysicsWorld::unscaleSpeed(scaledSpeed);
}

float Body2D::getAngularSpeed() const
{
    return m_body->GetAngularVelocity();
}

glm::vec2 Body2D::getLateralVelocity() const
{
    b2Vec2 currentRightNormal = m_body->GetWorldVector( b2Vec2(1,0) );
    b2Vec2 lateralVelocity = b2Dot(currentRightNormal, m_body->GetLinearVelocity()) * currentRightNormal;
    return { PhysicsWorld::unscaleSpeed(lateralVelocity.x), PhysicsWorld::unscaleSpeed(lateralVelocity.y) };
}

glm::vec2 Body2D::getForwardNormal() const
{
    const b2Vec2 forwardNormal = m_body->GetWorldVector(b2Vec2(0,1));
    return glm::vec2{ forwardNormal.x, forwardNormal.y };
}

float Body2D::getMass() const
{
    assert(m_body);
    return PhysicsWorld::unscaleMass(m_body->GetMass());
}

void Body2D::setMass(float mass)
{
    assert(m_body);
    b2MassData massData;
    m_body->GetMassData(&massData);
    massData.mass = PhysicsWorld::scaleMass(mass);
    m_body->SetMassData(&massData);

    /* Must also update top view friction */
    if (m_topViewFrictionJoint != nullptr) {
        setFrictionCoefficient(m_topViewFrictionCoefficient);
    }
}

void Body2D::setAngularDamping(float damping)
{
    m_body->SetAngularDamping(damping);
}

void Body2D::addTopViewFriction(float normalForce, float frictionCoefficient)
{
    b2BodyDef frictionBodyDef;
    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = true;
    m_frictionBody = m_world->CreateBody(&frictionBodyDef);
    b2FrictionJointDef jointDef;
    jointDef.bodyA = m_frictionBody;
    jointDef.bodyB = m_body;
    jointDef.maxForce = normalForce * frictionCoefficient;
    m_topViewFrictionCoefficient = frictionCoefficient;

    /* Don't use torque friction, it just causes weird physics behaviour */
    jointDef.maxTorque = 0.0f;
    /* No need to explicitly delete joint, it's deleted when the attached body is deleted */
    m_topViewFrictionJoint = static_cast<b2FrictionJoint *>(m_world->CreateJoint(&jointDef));
}

void Body2D::setFrictionCoefficient(float frictionCoefficient)
{
    assert(m_topViewFrictionJoint != nullptr);
    assert(0.0f <= frictionCoefficient && frictionCoefficient <= 1.0f);
    float normalForce = PhysicsWorld::normalForce(getMass());
    m_topViewFrictionJoint->SetMaxForce(normalForce * frictionCoefficient);
    m_topViewFrictionCoefficient = frictionCoefficient;
}

float Body2D::getTopViewFrictionForce(float stepTime) const
{
    const b2Vec2 reactionForce = m_topViewFrictionJoint->GetReactionForce(1.0f / stepTime);
    return PhysicsWorld::unscaleForce(reactionForce.Length());
}

