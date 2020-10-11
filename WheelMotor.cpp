#include "WheelMotor.h"
// TODO: Should I use iostream?
#include <iostream>
#include <chrono>
#include <ctime>

WheelMotor::WheelMotor(b2World* world, Type type, float width, float radius, float mass)
    : m_type(type)
    , m_radius(radius)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    //TODO: Fix scaling here instead..
    polygonShape.SetAsBox(width / 2.0f, radius);
    //TODO: Fix density
    m_body->CreateFixture(&polygonShape, mass / (2.0f * radius * width));
}

b2Vec2 WheelMotor::getLateralVelocity()
{
    b2Vec2 currentRightNormal = m_body->GetWorldVector( b2Vec2(1,0) );
    return b2Dot(currentRightNormal, m_body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 WheelMotor::getForwardVelocity()
{
    b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0,1));
    return b2Dot(currentForwardNormal, m_body->GetLinearVelocity()) * currentForwardNormal;
}

float WheelMotor::getForwardSpeed()
{
    b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0,1));
    return b2Dot(currentForwardNormal, m_body->GetLinearVelocity());
}

// NOTE: This is broken
float WheelMotor::getAcceleration(float speed)
{
    // GetLinearAcceleration doesn't seem to exist in the API

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
void WheelMotor::updateFriction(const Drive drive)
{
    // TODO: Check if this is needed when the wheel is static
    // Reduce angular rotation to make it act like a real wheel
    //float angularDamping = 1000; // Tweaked
    //m_body->ApplyAngularImpulse(angularDamping * m_body->GetInertia() * -m_body->GetAngularVelocity(), true);

    const b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0,1));
    const float currentForwardSpeed = getForwardSpeed(); // TODO: Compare with normalize forward vector
    const float angularSpeed = currentForwardSpeed / (3.14f * 2.0f * m_radius);

    // Must stop completely before applying torque in the opposite direction
    // To prevent the worst current spikes
    /* TODO: Move to control logic instead
    if ((currentForwardSpeed > m_minimumSpeed && m_voltageApplied < 0) || (currentForwardSpeed < -m_minimumSpeed && m_voltageApplied > 0)) {
        m_voltageApplied = 0;
    }
    */
    //const float current = (m_voltageApplied - backEMFVoltage) / m_motorResistance;
    //const float torqueApplied = current * m_torqueConstant;
    const float torqueApplied = m_voltageInConstant * m_voltageApplied - m_angularSpeedConstant * angularSpeed;

    const float rollingFriction = 0;
    /* Add constant friction when no voltage applied and above minimum speed
    bool belowMinimumSpeed = -m_minimumSpeed < currentForwardSpeed && currentForwardSpeed < m_minimumSpeed;
    // Simplification: Only apply rolling friction when we don't apply any voltage
    if (m_voltageApplied == 0 && !belowMinimumSpeed) {
        rollingFriction = (currentForwardSpeed > 0) ?  1000 : -1000;
    }
    */

    // Convert torque to force (t = r * F => F = t / r)
    const float forceApplied = (torqueApplied / m_radius) - rollingFriction;
    m_body->ApplyForce(forceApplied * currentForwardNormal, m_body->GetWorldCenter(), true);
    //std::cout << m_voltageApplied <<  " " << forceApplied << " " << currentForwardSpeed << "\n";

    // Cancel lateral velocity to prevent wheel from moving sideways
    // (mimmicks sideway friction)
    b2Vec2 lateralCancelingImpulse = m_body->GetMass() * -getLateralVelocity();

    // Allow some skidding
    if (lateralCancelingImpulse.Length() > m_maxLateralCancelingImpulse) {
        std::cout << "Skidding\n";
        lateralCancelingImpulse *= m_maxLateralCancelingImpulse / lateralCancelingImpulse.Length();
    }
    m_body->ApplyLinearImpulse(lateralCancelingImpulse, m_body->GetWorldCenter(), true);
}

// TODO: Remove
// This is for testing a standalone wheel
void WheelMotor::updateFrictionTest(const Drive drive)
{
    float forceToApply = 0;
    switch (drive) {
        case Drive::Forward:
            forceToApply = 10;
            break;
        case Drive::Backward:
            forceToApply = -10;
            break;
        case Drive::None:
            break;
    }
    b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0,1));
    m_body->ApplyForce(forceToApply * currentForwardNormal, m_body->GetWorldCenter(), true);
}

void WheelMotor::updateDrive(const Drive drive, float voltageApplied)
{
    switch (drive) {
        case Drive::Forward:
            m_voltageApplied = voltageApplied;
            break;
        case Drive::Backward:
            m_voltageApplied = -voltageApplied;
            break;
        case Drive::None:
            m_voltageApplied = 0;
            break;
    }
}

void WheelMotor::updateTurn(const Turn turn)
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
WheelMotor::~WheelMotor()
{
    m_body->GetWorld()->DestroyBody(m_body);
}
