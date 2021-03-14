#include "robots/PhysicsBot.h"

#include <glm/gtx/rotate_vector.hpp>
#include "components/Body2D.h"
#include "components/Transforms.h"
#include "components/RectComponent.h"
#include "shapes/RectObject.h"

#include <iostream>

PhysicsBot::PhysicsBot(Scene *scene, const glm::vec2 &size, const glm::vec2 &startPosition, const float startRotation) :
    SceneObject(scene)
{
    Body2D::Specification mainBodySpec(true, true, 0.42f);
    mainBodySpec.frictionCoefficient = 0.0f;
    m_transformComponent = std::make_unique<RectTransform>(startPosition, size, startRotation);
    const auto transform = static_cast<RectTransform *>(m_transformComponent.get());
    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, mainBodySpec);
    m_renderableComponent = std::make_unique<RectComponent>(transform, glm::vec4{1.0f,1.0f,1.0f,1.0f});

    auto wheelColor = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
    Body2D::Specification wheelSpec(true, true, 0.125f);
    glm::vec2 wheelSize{0.015, 0.02};
    glm::vec2 wheelPosFrontRight{((size.x + 0.015f) / 2.0f), size.y/4.0f};
    glm::vec2 wheelPosBackRight{((size.x + 0.015f) / 2.0f), -size.y/4.0f};
    glm::vec2 wheelPosFrontLeft{-((size.x + 0.015f) / 2.0f), size.y/4.0f};
    glm::vec2 wheelPosBackLeft{-((size.x + 0.015f) / 2.0f), -size.y/4.0f};
    m_frontLeftWheel = std::make_unique<RectObject>(scene, wheelColor, &wheelSpec, wheelPosFrontLeft, wheelSize, 0);
    m_backLeftWheel = std::make_unique<RectObject>(scene, wheelColor, &wheelSpec, wheelPosBackLeft, wheelSize, 0);
    m_frontRightWheel = std::make_unique<RectObject>(scene, wheelColor, &wheelSpec, wheelPosFrontRight, wheelSize, 0);
    m_backRightWheel = std::make_unique<RectObject>(scene, wheelColor, &wheelSpec, wheelPosBackRight, wheelSize, 0);

    static_cast<Body2D *>(m_physicsComponent.get())->attachBodyWithRevoluteJoint(wheelPosFrontLeft, m_frontLeftWheel->getBody());
    static_cast<Body2D *>(m_physicsComponent.get())->attachBodyWithRevoluteJoint(wheelPosBackLeft, m_backLeftWheel->getBody());
    static_cast<Body2D *>(m_physicsComponent.get())->attachBodyWithRevoluteJoint(wheelPosFrontRight, m_frontRightWheel->getBody());
    static_cast<Body2D *>(m_physicsComponent.get())->attachBodyWithRevoluteJoint(wheelPosBackRight, m_backRightWheel->getBody());

}

PhysicsBot::~PhysicsBot()
{
}

void PhysicsBot::onFixedUpdate()
{
    auto frontLeftWheelBody = m_frontLeftWheel->getBody();
    auto backLeftWheelBody = m_backLeftWheel->getBody();
    auto frontRightWheelBody = m_frontRightWheel->getBody();
    auto backRightWheelBody = m_backRightWheel->getBody();

    setMotorForce(frontLeftWheelBody, m_frontLeftVoltage);
    setMotorForce(backLeftWheelBody, m_backLeftVoltage);
    setMotorForce(frontRightWheelBody, m_frontRightVoltage);
    setMotorForce(backRightWheelBody, m_backRightVoltage);

    glm::vec2 lateralImpulseFrontLeft = -frontLeftWheelBody->getLateralVelocity() * 1500.0f;
    frontLeftWheelBody->setLinearImpulse(lateralImpulseFrontLeft);
    glm::vec2 lateralImpulseBackLeft = -backLeftWheelBody->getLateralVelocity() * 1500.0f;
    backLeftWheelBody->setLinearImpulse(lateralImpulseBackLeft);
    glm::vec2 lateralImpulseFrontRight = -frontRightWheelBody->getLateralVelocity() * 1500.0f;
    frontLeftWheelBody->setLinearImpulse(lateralImpulseFrontRight);
    glm::vec2 lateralImpulseBackRight = -backRightWheelBody->getLateralVelocity() * 1500.0f;
    backRightWheelBody->setLinearImpulse(lateralImpulseBackRight);

    /* TODO: The sideway friction should depend on the weight of the bot */
}

void PhysicsBot::setVoltageFrontLeft(float voltage)
{
    m_frontLeftVoltage = voltage;
}

void PhysicsBot::setVoltageBackLeft(float voltage)
{
    m_backLeftVoltage = voltage;
}

void PhysicsBot::setVoltageFrontRight(float voltage)
{
    m_frontRightVoltage = voltage;
}

void PhysicsBot::setVoltageBackRight(float voltage)
{
    m_backRightVoltage = voltage;
}

void PhysicsBot::setForceFrontLeft(float force)
{
    auto frontLeftWheelBody = m_frontLeftWheel->getBody();
    frontLeftWheelBody->setForce(frontLeftWheelBody->getForwardNormal(), force);
}

void PhysicsBot::setForceBackLeft(float force)
{
    auto backLeftWheelBody = m_backLeftWheel->getBody();
    backLeftWheelBody->setForce(backLeftWheelBody->getForwardNormal(), force);
}

void PhysicsBot::setForceFrontRight(float force)
{
    auto frontRightWheelBody = m_frontRightWheel->getBody();
    frontRightWheelBody->setForce(frontRightWheelBody->getForwardNormal(), force);
}

void PhysicsBot::setForceBackRight(float force)
{
    auto backRightWheelBody = m_backRightWheel->getBody();
    backRightWheelBody->setForce(backRightWheelBody->getForwardNormal(), force);
}

void PhysicsBot::setMotorForce(Body2D *body2D, float voltageIn)
{
    const float diameter = 0.03f;
    const float voltageInConstant = 0.00528f;
    const float angularSpeedConstant = 0.00178f;

    const glm::vec2 currentForwardNormal = body2D->getForwardNormal();
    const float currentForwardSpeed = body2D->getForwardSpeed();
    const float angularSpeed = currentForwardSpeed / (3.14f * diameter);
    const float torqueApplied = voltageInConstant * voltageIn - angularSpeedConstant * angularSpeed;
    const float rollingFriction = 0;
    /* Convert torque to force (t = r * F => F = t / r) */
    const float forceToApply = (torqueApplied / (diameter / 2)) - rollingFriction;
    body2D->setForce(currentForwardNormal, forceToApply);

    /* Apply sideway friction to mimic real wheel */
    #if 0
    glm::vec2 lateralCancelingImpulse = -m_body2D->getLateralVelocity() * m_spec.sidewayFrictionConstant;
    m_body2D->setLinearImpulse(lateralCancelingImpulse);
    #endif
}
