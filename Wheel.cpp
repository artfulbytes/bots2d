#include "Wheel.h"
// TODO: Should I use iostream?
#include <iostream>
#include <chrono>
#include <ctime>

Wheel::Wheel(b2World* world, Type type, float width, float length, float weight)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    //TODO: Fix scaling here instead..
    polygonShape.SetAsBox(width / 2.0f, length / 2.0f);
    //TODO: Fix density
    m_body->CreateFixture(&polygonShape, weight / (length * width));//shape density

    m_wheelType = type;
}

void Wheel::setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed,
                               float maxDriveForce, float maxLateralImpulse)
{
    // TODO: Do scaling here?
    m_maxForwardSpeed = maxForwardSpeed;
    m_maxBackwardSpeed = maxBackwardSpeed;
    m_maxDriveForce = maxDriveForce;
    m_maxLateralImpulse = maxLateralImpulse;
}

b2Vec2 Wheel::getLateralVelocity()
{
    b2Vec2 currentRightNormal = m_body->GetWorldVector( b2Vec2(1,0) );
    return b2Dot(currentRightNormal, m_body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 Wheel::getForwardVelocity()
{
    b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0,1));
    return b2Dot(currentForwardNormal, m_body->GetLinearVelocity()) * currentForwardNormal;
}

// TODO: Test function, remove?
float Wheel::getAcceleration(float speed)
{
    static auto startTime = std::chrono::system_clock::now();
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<float> timeElapsed;
    static float lastSpeed = 0.0f;
    static float acceleration = 0.0f;

    timeElapsed = endTime-startTime;

    if (timeElapsed.count() > 1) {
        acceleration = (speed - lastSpeed) / timeElapsed.count();
        startTime = std::chrono::system_clock::now();
        lastSpeed = speed;
    }

    return acceleration;
}

// Make the body act like a wheel
void Wheel::updateFriction()
{
    // Allow some skidding
    //float maxLateralImpulse = 10.5f; // Tweaked
    b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
    //if (impulse.Length() > maxLateralImpulse) {
        //impulse *= maxLateralImpulse / impulse.Length();
    //}
    // Cancel lateral velocity to prevent wheel from moving sideways
    m_body->ApplyLinearImpulse(impulse, m_body->GetWorldCenter(), true);

    // Reduce angular rotation to make it act like tire
    float angularDamping = 1; // Tweaked
    m_body->ApplyAngularImpulse(angularDamping * m_body->GetInertia() * -m_body->GetAngularVelocity(), true);

    // Add drag force to the wheel so it doesn't roll forever
    b2Vec2 currentForwardNormal = getForwardVelocity();
    float currentForwardSpeed = currentForwardNormal.Normalize();
    float dragFactor = 100; // Tweaked
    float dragForceMagnitude = -dragFactor * currentForwardSpeed;
    m_body->ApplyForce(dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter(), true);
}


// Currently: Just apply max forward / backward speed (force)
void Wheel::updateDrive(const Drive drive)
{
    float desiredSpeed = 0;
    switch (drive) {
        case Drive::Forward:
            desiredSpeed = m_maxForwardSpeed;
            break;
        case Drive::Backward:
            desiredSpeed = m_maxBackwardSpeed;
            break;
        case Drive::None:
            break;
    }

    b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0,1));
    float currentForwardSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);
    //std::cout << "Acc: " << getAcceleration(currentForwardSpeed) << "\n";

    updateTurn(Turn::Left);

    float forceToApply = 0;
    // TODO: What about backward speed here?
    if (desiredSpeed > currentForwardSpeed)
        forceToApply = m_maxDriveForce;
    else if (desiredSpeed < currentForwardSpeed)
        forceToApply = -m_maxDriveForce;
    else
        forceToApply = 0;
    // TODO: Scale here or scale m_maxDriveForce?
    m_body->ApplyForce(forceToApply * currentForwardNormal, m_body->GetWorldCenter(), true);
}

void Wheel::updateTurn(const Turn turn)
{
    float desiredTorque = 0;
    switch (turn) {
        case Turn::Left:
            desiredTorque = 15;
            break;
        case Turn::Right:
            desiredTorque = -15;
            break;
        case Turn::None:
            break;
    }
    m_body->ApplyTorque(desiredTorque, true);
}

// TODO: Where to place {?
Wheel::~Wheel()
{
    m_body->GetWorld()->DestroyBody(m_body);
}
