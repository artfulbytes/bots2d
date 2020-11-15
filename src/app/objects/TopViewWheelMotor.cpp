#include "TopViewWheelMotor.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include <glm/glm.hpp>

TopViewWheelMotor::TopViewWheelMotor(AppScene &appScene, const PhysicsWorld &world, const Specification &spec, Vec2 startPos) :
    AppObject(appScene),
    m_spec(spec)
{
    /* TODO: Make vecs passable via constructor on transforms */
    QuadTransform *transformBody = new QuadTransform();
    transformBody->position.x = startPos.x;
    transformBody->position.y = startPos.y;
    transformBody->position.z = 0.0f;
    transformBody->size.x = spec.width;
    transformBody->size.y = spec.diameter;
    assert(m_spec.maxVoltage > 0);

    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    QuadComponent *renderable = new QuadComponent(color);
    m_body2D = new Body2D(world, *transformBody, true, spec.mass);
    /* TODO: Make this access less ugly */
    appScene.getScene()->createObject(transformBody, renderable, m_body2D, nullptr);
    scaleSpecs(m_spec);
}

void TopViewWheelMotor::scaleSpecs(Specification &spec)
{
    spec.diameter = PhysicsWorld::scaleLength(spec.diameter);
    spec.width = PhysicsWorld::scaleLength(spec.width);
    spec.mass = PhysicsWorld::scaleMass(spec.mass);
    spec.maxDriveForce = PhysicsWorld::scaleForce(spec.maxDriveForce);
}

void TopViewWheelMotor::setVoltageIn(float voltage)
{
    assert(abs(voltage) <= m_spec.maxVoltage);
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
    const float angularSpeed = currentForwardSpeed / (3.14f * m_spec.diameter);

    // Must stop completely before applying torque in the opposite direction
    // To prevent the worst current spikes
    /* TODO: Move to control logic instead
    if ((currentForwardSpeed > m_minimumSpeed && m_voltageApplied < 0) || (currentForwardSpeed < -m_minimumSpeed && m_voltageApplied > 0)) {
        m_voltageApplied = 0;
    }
    */
    const float torqueApplied = m_spec.voltageInConstant * m_voltageIn - m_spec.angularSpeedConstant * angularSpeed;

    const float rollingFriction = 0;
    /* Add constant friction when no voltage applied and above minimum speed
    bool belowMinimumSpeed = -m_minimumSpeed < currentForwardSpeed && currentForwardSpeed < m_minimumSpeed;
    // Simplification: Only apply rolling friction when we don't apply any voltage
    if (m_voltageApplied == 0 && !belowMinimumSpeed) {
        rollingFriction = (currentForwardSpeed > 0) ?  1000 : -1000;
    }
    */

    // Convert torque to force (t = r * F => F = t / r)
    const float forceToApply = (torqueApplied / (m_spec.diameter / 2)) - rollingFriction;

    m_body2D->setForce(currentForwardNormal, forceToApply);
    //std::cout << m_voltageApplied <<  " " << forceApplied << " " << currentForwardSpeed << "\n";

    // Cancel lateral velocity to prevent wheel from moving sideways
    // (mimmicks sideway friction)
    //b2Vec2 lateralCancelingImpulse = m_body->GetMass() * -getLateralVelocity();

    // Allow some skidding
    //if (lateralCancelingImpulse.Length() > m_maxLateralCancelingImpulse) {
        //std::cout << "Skidding\n";
        //lateralCancelingImpulse *= m_maxLateralCancelingImpulse / lateralCancelingImpulse.Length();
    //}
    //m_body->ApplyLinearImpulse(lateralCancelingImpulse, m_body->GetWorldCenter(), true);
}

void TopViewWheelMotor::onFixedUpdate(double stepTime)
{
    updateForce();
}

TopViewWheelMotor::~TopViewWheelMotor()
{
}

