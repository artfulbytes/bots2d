#include "TopViewWheelMotor.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include <glm/glm.hpp>
#include <iostream>

TopViewWheelMotor::TopViewWheelMotor(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2 &unscaledStartPos) :
    AppObject(appScene),
    m_scaledSpec(scaleSpec(unscaledSpec))
{
    assert(world.getGravityType() == PhysicsWorld::Gravity::TopView);
    assert(unscaledSpec.maxVoltage > 0);
    /* TODO: Make vecs passable via constructor on transforms */
    QuadTransform *transformBody = new QuadTransform();
    transformBody->position.x = unscaledStartPos.x;
    transformBody->position.y = unscaledStartPos.y;
    transformBody->position.z = 0.0f;
    transformBody->size.x = unscaledSpec.width;
    transformBody->size.y = unscaledSpec.diameter;

    glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
    QuadComponent *renderable = new QuadComponent(color);
    m_body2D = new Body2D(world, *transformBody, true, true, unscaledSpec.mass);
    /* TODO: Make this access less ugly */
    appScene.getScene()->createObject(transformBody, renderable, m_body2D, nullptr);
}

TopViewWheelMotor::Specification TopViewWheelMotor::scaleSpec(const Specification &unscaledSpec)
{
    const Specification scaledSpec = {
        .voltageInConstant = unscaledSpec.voltageInConstant,
        .angularSpeedConstant = unscaledSpec.angularSpeedConstant,
        .maxVoltage = unscaledSpec.maxVoltage,
        .diameter = PhysicsWorld::scaleLength(unscaledSpec.diameter),
        .width = PhysicsWorld::scaleLength(unscaledSpec.width),
        .mass = PhysicsWorld::scaleMass(unscaledSpec.mass),
        .maxDriveForce = PhysicsWorld::scaleForce(unscaledSpec.maxDriveForce),
        .maxLateralCancelingImpulse = unscaledSpec.maxLateralCancelingImpulse
    };
    return scaledSpec;
}

void TopViewWheelMotor::setVoltageIn(float voltage)
{
    assert(abs(voltage) <= m_scaledSpec.maxVoltage);
    m_voltageIn = voltage;
    updateForce();
}

float *TopViewWheelMotor::getVoltageLine()
{
    return &m_voltageIn;
}

void TopViewWheelMotor::updateForce()
{
    const Vec2 currentForwardNormal = m_body2D->getForwardNormal();
    const float currentForwardSpeed = m_body2D->getForwardSpeed(); // TODO: Compare with normalize forward vector
    const float angularSpeed = currentForwardSpeed / (3.14f * m_scaledSpec.diameter);

    // Must stop completely before applying torque in the opposite direction
    // To prevent the worst current spikes
    /* TODO: Move to control logic instead
    if ((currentForwardSpeed > m_minimumSpeed && m_voltageApplied < 0) || (currentForwardSpeed < -m_minimumSpeed && m_voltageApplied > 0)) {
        m_voltageApplied = 0;
    }
    */
    const float torqueApplied = m_scaledSpec.voltageInConstant * m_voltageIn - m_scaledSpec.angularSpeedConstant * angularSpeed;

    const float rollingFriction = 0;
    /* Add constant friction when no voltage applied and above minimum speed
    bool belowMinimumSpeed = -m_minimumSpeed < currentForwardSpeed && currentForwardSpeed < m_minimumSpeed;
    // Simplification: Only apply rolling friction when we don't apply any voltage
    if (m_voltageApplied == 0 && !belowMinimumSpeed) {
        rollingFriction = (currentForwardSpeed > 0) ?  1000 : -1000;
    }
    */

    // Convert torque to force (t = r * F => F = t / r)
    const float forceToApply = (torqueApplied / (m_scaledSpec.diameter / 2)) - rollingFriction;

    m_body2D->setForce(currentForwardNormal, forceToApply);
    //std::cout << m_voltageApplied <<  " " << forceApplied << " " << currentForwardSpeed << "\n";

    // Cancel lateral velocity to prevent wheel from moving sideways
    // (mimmicks sideway friction)
    m_body2D->getLateralVelocity();
    m_body2D->getMass();

    Vec2 lateralCancelingImpulse = m_body2D->getLateralVelocity();
    lateralCancelingImpulse.x *= -m_body2D->getMass();
    lateralCancelingImpulse.y *= -m_body2D->getMass();

    // Allow some skidding
    if (lateralCancelingImpulse.length() > m_scaledSpec.maxLateralCancelingImpulse) {
        std::cout << "Skidding" << std::endl;
        lateralCancelingImpulse.x *= m_scaledSpec.maxLateralCancelingImpulse / lateralCancelingImpulse.length();
        lateralCancelingImpulse.y *= m_scaledSpec.maxLateralCancelingImpulse / lateralCancelingImpulse.length();
    }
    m_body2D->setLinearImpulse(lateralCancelingImpulse);
}

void TopViewWheelMotor::onFixedUpdate(double stepTime)
{
    updateForce();
}

TopViewWheelMotor::~TopViewWheelMotor()
{
}

