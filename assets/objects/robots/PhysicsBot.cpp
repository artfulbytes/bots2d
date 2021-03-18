#include "robots/PhysicsBot.h"

#include <glm/gtx/rotate_vector.hpp>
#include "components/Body2D.h"
#include "components/Transforms.h"
#include "components/RectComponent.h"
#include "shapes/RectObject.h"
#include "Scene.h"

#include <iostream>

PhysicsBot::PhysicsBot(Scene *scene, const glm::vec2 &size, const glm::vec2 &startPosition, const float startRotation) :
    SceneObject(scene), m_frictionCoefficient(0.1f), m_bodyMass(0.4f), m_wheelMass(0.025f), m_wheelCount(4)
{
    Body2D::Specification mainBodySpec(true, true, m_bodyMass, 0.0f);
    m_transformComponent = std::make_unique<RectTransform>(startPosition, size, startRotation);
    const auto transform = static_cast<RectTransform *>(m_transformComponent.get());
    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, mainBodySpec);
    m_renderableComponent = std::make_unique<RectComponent>(transform, glm::vec4{1.0f,1.0f,1.0f,1.0f});
    m_body = static_cast<Body2D *>(m_physicsComponent.get());

    auto wheelColor = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
    Body2D::Specification wheelSpec(true, true, getLoadedWheelMass());
    wheelSpec.frictionCoefficient = m_frictionCoefficient;
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

    const float epsilon = 0.001f;
    /* Only do callbacks every 10 ms (100 times per second) */
    const auto millisecondsSinceStart = m_scene->getMillisecondsSinceStart();
    if (millisecondsSinceStart - m_lastCallbackTime > 10) {
        const auto forwardSpeed = getForwardSpeed();
        if (fabs(forwardSpeed) > (epsilon + fabs(m_recordedTopSpeed))) {
            m_recordedTopSpeed = fabs(forwardSpeed);
        }
        if (fabs(forwardSpeed) < (epsilon / 10.0f)) {
            m_lastStandStillTime = millisecondsSinceStart;
        }

        /* Are we at top speed? */
        const bool atTopSpeed = fabs(forwardSpeed - m_recordedTopSpeed) < epsilon;
        if (atTopSpeed) {
            float accelerationToTopSpeed = 1000.0f * fabs(forwardSpeed) / (millisecondsSinceStart - m_lastStandStillTime);
            const bool atNewTopSpeed = fabs(forwardSpeed) > (fabs(m_bestAccelerationRecordedAtSpeed) + 0.01f);
            if (atNewTopSpeed) {
                m_bestAccelerationToCurrentTopSpeed = accelerationToTopSpeed;
                m_timeToTopSpeed = (millisecondsSinceStart - m_lastStandStillTime) / 1000.0f;
                m_bestAccelerationRecordedAtSpeed = fabs(forwardSpeed);
            } else {
                const bool newAccelerationIsBetter = accelerationToTopSpeed > m_bestAccelerationToCurrentTopSpeed;
                if (newAccelerationIsBetter) {
                    m_bestAccelerationToCurrentTopSpeed = accelerationToTopSpeed;
                    m_timeToTopSpeed = (millisecondsSinceStart - m_lastStandStillTime) / 1000.0f;
                }
            }
        }

        if (m_onForwardSpeedChanged) {
            m_onForwardSpeedChanged(forwardSpeed);
        }

        if (m_onTopSpeedChanged) {
            m_onTopSpeedChanged(m_recordedTopSpeed);
        }

        if (m_onAccelerationToTopSpeedChanged) {
            m_onAccelerationToTopSpeedChanged(m_bestAccelerationToCurrentTopSpeed);
        }

        const float forwardAcceleration = 1000.0f * (forwardSpeed - m_lastForwardSpeed) / (millisecondsSinceStart - m_lastCallbackTime);
        if (m_onForwardAccelerationChanged) {
            m_onForwardAccelerationChanged(forwardAcceleration);
        }
        m_lastForwardSpeed = forwardSpeed;
        m_lastCallbackTime = millisecondsSinceStart;
    }
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
    glm::vec2 lateralCancelingImpulse = -body2D->getLateralVelocity() * m_sidewayFrictionConstant;
    /* TODO: The sideway friction should depend on the weight of the bot */
    body2D->setLinearImpulse(lateralCancelingImpulse);
}

void PhysicsBot::setSidewayFrictionConstant(float sidewayFrictionConstant)
{
    m_sidewayFrictionConstant = sidewayFrictionConstant;
}

void PhysicsBot::setFrictionCoefficient(float frictionCoefficient)
{
    m_frictionCoefficient = frictionCoefficient;
    m_frontLeftWheel->getBody()->setFrictionCoefficient(frictionCoefficient);
    m_backLeftWheel->getBody()->setFrictionCoefficient(frictionCoefficient);
    m_frontRightWheel->getBody()->setFrictionCoefficient(frictionCoefficient);
    m_backRightWheel->getBody()->setFrictionCoefficient(frictionCoefficient);
}

void PhysicsBot::setTotalMass(float mass)
{
    assert(mass > 0.0f);
    m_wheelMass = (mass * 0.2f) / m_wheelCount;
    m_bodyMass = mass - (m_wheelMass * m_wheelCount);
    assert(fabs(mass - getTotalMass()) < 0.001f);
    m_frontLeftWheel->getBody()->setMass(m_wheelMass);
    m_frontRightWheel->getBody()->setMass(m_wheelMass);
    m_backLeftWheel->getBody()->setMass(m_wheelMass);
    m_backRightWheel->getBody()->setMass(m_wheelMass);
    m_body->setMass(m_bodyMass);
}

float PhysicsBot::getForwardSpeed() const
{
    return m_body->getForwardSpeed();
}

void PhysicsBot::setForwardSpeedCallback(std::function<void(float)> onForwardSpeedChanged)
{
    m_onForwardSpeedChanged = onForwardSpeedChanged;
}

void PhysicsBot::setForwardAccelerationCallback(std::function<void(float)> onForwardAccelerationChanged)
{
    m_onForwardAccelerationChanged = onForwardAccelerationChanged;
}

void PhysicsBot::setTopSpeedCallback(std::function<void(float)> onTopSpeedChanged)
{
    m_onTopSpeedChanged = onTopSpeedChanged;
}

void PhysicsBot::setAccelerationToTopSpeedCallback(std::function<void(float)> onAccelerationToTopSpeedChanged)
{
    m_onAccelerationToTopSpeedChanged = onAccelerationToTopSpeedChanged;
}

void PhysicsBot::resetRecordedValues()
{
    m_recordedTopSpeed = 0.0f;
    m_topSpeedAcceleration = 0.0f;
    m_bestAccelerationToCurrentTopSpeed = 0.0f;
    m_bestAccelerationRecordedAtSpeed = 0.0f;
    m_timeToTopSpeed = 0.0f;
}
