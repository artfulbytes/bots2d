#include "robots/BaseBot.h"

#include "sensors/LineDetectorObject.h"
#include "components/KeyboardController.h"
#include "Scene.h"
#include "components/Body2D.h"

#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

void BaseBot::sanityCheckSpec(const BaseBot::Specification &spec)
{
    assert(0.02f <= spec.bodyWidth && spec.bodyWidth <= 0.3f);
    assert(0.02f <= spec.bodyLength && spec.bodyLength <= 0.3f);
    assert(0.005f <= spec.wheelWidth && spec.wheelWidth <= 0.1f);
    assert(0.015f <= spec.wheelDiameter && spec.wheelDiameter <= 0.1f);
    for (const auto &wheelMotorTuple : spec.wheelMotorTuples) {
        const auto position = std::get<1>(wheelMotorTuple);
        const float minPosX = (spec.bodyWidth + spec.wheelWidth) / 2;
        const float maxPosY = spec.bodyLength / 2;
        /* Must lie outside the body */
        assert(minPosX <= std::abs(position.x));
        /* Middle of wheel shouldn't pass beyond body */
        assert(std::abs(position.y) <= maxPosY);
    }
}

BaseBot::BaseBot(Scene *scene, const BaseBot::Specification &spec,
                 const glm::vec2 &startPosition, const float startRotation) :
    SceneObject(scene)
{
    sanityCheckSpec(spec);
    createBody(spec, startPosition, startRotation);
    createWheelMotors(spec);
    createSensors(spec);
}

BaseBot::~BaseBot()
{
}

void BaseBot::createBody(const BaseBot::Specification &spec, const glm::vec2 &startPosition, float startRotation)
{
    const SimpleBotBody::Specification bodySpec(spec.bodyLength, spec.bodyWidth, spec.bodyMass,
                                              spec.bodyShape, spec.bodyTexture, spec.angularDamping);
    m_body = std::make_unique<SimpleBotBody>(m_scene, bodySpec, startPosition, startRotation);
}

void BaseBot::createWheelMotors(const BaseBot::Specification &spec)
{
    const auto wheelCount = spec.wheelMotorTuples.size();
    const float loadedMass = spec.bodyMass / wheelCount;
    const WheelMotor::Specification wheelSpec(spec.motorVoltageInConstant, spec.motorAngularSpeedConstant,
                                              spec.motorMaxVoltage, spec.wheelFrictionCoefficient, spec.wheelSidewayFrictionConstant,
                                              spec.wheelWidth, spec.wheelDiameter, spec.wheelMass, loadedMass,
                                              spec.wheelTexture);
    for (const auto &wheelMotorTuple : spec.wheelMotorTuples) {
        const auto index = std::get<0>(wheelMotorTuple);
        const auto relativePosition = std::get<1>(wheelMotorTuple);
        const auto rotation = m_body->getRotation();
        const auto absolutePosition = m_body->getPosition() + glm::rotate(relativePosition, rotation);
        switch (index) {
        case BaseBot::WheelMotorIndex::Left:
        case BaseBot::WheelMotorIndex::FrontLeft:
        case BaseBot::WheelMotorIndex::BackLeft:
            m_wheelMotors[index] = std::make_unique<WheelMotor>(m_scene, wheelSpec, WheelMotor::Orientation::Left,
                                                                absolutePosition, rotation);
            break;
        case BaseBot::WheelMotorIndex::Right:
        case BaseBot::WheelMotorIndex::FrontRight:
        case BaseBot::WheelMotorIndex::BackRight:
            m_wheelMotors[index] = std::make_unique<WheelMotor>(m_scene, wheelSpec, WheelMotor::Orientation::Right,
                                                                absolutePosition, rotation);
            break;
        }
        m_body->attachWheelMotor(m_wheelMotors[index].get(), relativePosition);
    }
}

void BaseBot::createSensors(const BaseBot::Specification &spec)
{
    for (const auto &rangeSensorTuple : spec.rangeSensorTuples) {
        const auto index = std::get<0>(rangeSensorTuple);
        const auto relativePosition = std::get<1>(rangeSensorTuple);
        const auto absolutePosition = m_body->getPosition() + glm::rotate(relativePosition, m_body->getRotation());
        const auto rangeSensorSpec = std::get<2>(rangeSensorTuple);
        m_rangeSensors[index] = std::make_unique<RangeSensorObject>(m_scene, rangeSensorSpec, false, absolutePosition);
        m_body->attachSensor(m_rangeSensors[index].get(), relativePosition);
    }
    for (const auto &lineDetectorTuple : spec.lineDetectorTuples) {
        const auto index = std::get<0>(lineDetectorTuple);
        const auto relativePosition = std::get<1>(lineDetectorTuple);
        const auto absolutePosition = m_body->getPosition() + glm::rotate(relativePosition, m_body->getRotation());
        m_lineDetectors[index] = std::make_unique<LineDetectorObject>(m_scene, false, absolutePosition);
        m_body->attachSensor(m_lineDetectors[index].get(), relativePosition);
    }
}

void BaseBot::setDebug(bool enabled)
{
    for (const auto &lineDetectorTuple : m_lineDetectors) {
        lineDetectorTuple.second->setDebugDraw(enabled);
    }
    for (const auto &rangeSensorTuple : m_rangeSensors) {
        rangeSensorTuple.second->setDebugDraw(enabled);
    }
    m_debugEnabled = enabled;
}

float *BaseBot::getVoltageLine(BaseBot::WheelMotorIndex wheelMotorIndex) const
{
    auto wheelMotorItr = m_wheelMotors.find(wheelMotorIndex);
    if (wheelMotorItr == m_wheelMotors.end()) {
        std::cout << "Wheel motor not found!" << std::endl;
        assert(0);
    }
    return wheelMotorItr->second->getVoltageLine();
}

float *BaseBot::getVoltageLine(BaseBot::RangeSensorIndex rangeSensorIndex) const
{
    auto rangeSensorItr = m_rangeSensors.find(rangeSensorIndex);
    if (rangeSensorItr == m_rangeSensors.end()) {
        std::cout << "Range sensor not found!" << std::endl;
        assert(0);
    }
    return rangeSensorItr->second->getVoltageLine();
}

float *BaseBot::getVoltageLine(BaseBot::LineDetectorIndex lineDetectorIndex) const
{
    auto lineDetectorItr = m_lineDetectors.find(lineDetectorIndex);
    if (lineDetectorItr == m_lineDetectors.end()) {
        std::cout << "Line detector not found!" << std::endl;
        assert(0);
    }
    return lineDetectorItr->second->getVoltageLine();
}

void BaseBot::onFixedUpdate()
{
    /* Only do callbacks every 10 ms (100 times per second) */
    const auto millisecondsSinceStart = m_scene->getMillisecondsSinceStart();
    const float epsilon = 0.01f;
    if (millisecondsSinceStart - m_lastCallbackTime > 10) {
        const auto forwardSpeed = getForwardSpeed();
        bool newTopSpeed = false;
        if (fabs(forwardSpeed) > (epsilon + fabs(m_recordedTopSpeed))) {
            newTopSpeed = true;
            m_recordedTopSpeed = fabs(forwardSpeed);
        }
        if (fabs(forwardSpeed) < (epsilon / 10.0f)) {
            m_lastFwdStandStillTime = millisecondsSinceStart;
            m_wasAtStandStill = true;
        }

        const bool atTopSpeed = fabs(forwardSpeed - m_recordedTopSpeed) < epsilon && forwardSpeed > epsilon;
        bool justReachedTopSpeed = false;
        if (atTopSpeed && !m_wasAtTopSpeed) {
            justReachedTopSpeed = true;
        }
        m_wasAtTopSpeed = atTopSpeed;

        if (newTopSpeed || (justReachedTopSpeed && m_wasAtStandStill)) {
            m_timeToReachTopSpeed = (millisecondsSinceStart - m_lastFwdStandStillTime) / 1000.0f;
            m_topSpeedAcceleration = forwardSpeed / m_timeToReachTopSpeed;
            m_wasAtStandStill = false;
        }

        const float forwardAcceleration = 1000.0f * (forwardSpeed - m_lastForwardSpeed) / (millisecondsSinceStart - m_lastCallbackTime);
        if (forwardAcceleration > 0 && forwardAcceleration > (epsilon + m_recordedTopAcceleration)) {
            m_recordedTopAcceleration = fabs(forwardAcceleration);
        }

        if (m_onForwardAccelerationChanged) {
            m_onForwardAccelerationChanged(forwardAcceleration);
        }

        if (m_onTopSpeedChanged) {
            m_onTopSpeedChanged(m_recordedTopSpeed);
        }
        if (m_onForwardSpeedChanged) {
            m_onForwardSpeedChanged(forwardSpeed);
        }

        if (m_onTimeToTopSpeedChanged) {
            m_onTimeToTopSpeedChanged(m_timeToReachTopSpeed);
        }

        if (m_onTopSpeedAccelerationChanged) {
            m_onTopSpeedAccelerationChanged(m_topSpeedAcceleration);
        }

        if (m_onTopAccelerationChanged) {
            m_onTopAccelerationChanged(m_recordedTopAcceleration);
        }


        if (m_onTimeMovingChanged) {
            // It's enough to check angular speed (it's also > 0 when driving forward)
            const bool isMoving = fabs(m_body->getAngularSpeed()) > (epsilon / 1000);

            if (isMoving) {
                if (m_wasAtRotStandStill) {
                    m_lastRotStandStillTime = millisecondsSinceStart;
                    m_wasAtRotStandStill = false;
                }
            } else  {
                m_wasAtRotStandStill = true;
            }
            const auto lastStandStillTime = m_lastRotStandStillTime;
            if (isMoving) {
                m_onTimeMovingChanged(millisecondsSinceStart - lastStandStillTime);
            }
        }

        m_lastForwardSpeed = forwardSpeed;
        m_lastCallbackTime = millisecondsSinceStart;
    }
}

void BaseBot::setWheelFrictionCoefficient(float frictionCoefficient)
{
    for (auto &wheelMotor : m_wheelMotors)
    {
        wheelMotor.second->setFrictionCoefficient(frictionCoefficient);
    }
}

void BaseBot::setWheelSidewayFrictionConstant(float sidewayFrictionConstant)
{
    for (auto &wheelMotor: m_wheelMotors)
    {
        wheelMotor.second->setSidewayFrictionConstant(sidewayFrictionConstant);
    }
}

float BaseBot::getWheelFrictionCoefficient() const
{
    return m_wheelMotors.begin()->second->getFrictionCoefficient();
}

float BaseBot::getWheelSidewayFrictionConstant() const
{
    return m_wheelMotors.begin()->second->getSidewayFrictionConstant();
}

void BaseBot::setWheelMass(float mass)
{
    for (auto &wheelMotor : m_wheelMotors) {
        wheelMotor.second->setMass(mass);
    }
}

void BaseBot::setBodyMass(float mass)
{
    m_body->setMass(mass);
    const auto wheelCount = m_wheelMotors.size();
    for (auto &wheelMotor : m_wheelMotors) {
        wheelMotor.second->setLoadedMass(mass / wheelCount);
    }
}

float BaseBot::getBodyMass() const
{
    return m_body->getMass();
}

float BaseBot::getWheelMass() const
{
    return m_wheelMotors.begin()->second->getMass();
}

void BaseBot::setMotorMaxVoltage(float maxVoltage)
{
    for (auto &wheelMotor : m_wheelMotors) {
        wheelMotor.second->setMaxVoltage(maxVoltage);
    }
}

void BaseBot::setMotorAngularSpeedConstant(float angularSpeedConstant)
{
    for (auto &wheelMotor : m_wheelMotors) {
        wheelMotor.second->setAngularSpeedConstant(angularSpeedConstant);
    }
}

void BaseBot::setMotorVoltageInConstant(float voltageInConstant)
{
    for (auto &wheelMotor : m_wheelMotors) {
        wheelMotor.second->setVoltageInConstant(voltageInConstant);
    }
}

float BaseBot::getMotorVoltageInConstant() const
{
    /* All motors have the same value, just pick the first one */
    return m_wheelMotors.begin()->second->getVoltageInConstant();
}

float BaseBot::getMotorMaxVoltage() const
{
    /* All motors have the same value, just pick the first one */
    return m_wheelMotors.begin()->second->getMaxVoltage();
}

float BaseBot::getMotorAngularSpeedConstant() const
{
    /* All motors have the same value, just pick the first one */
    return m_wheelMotors.begin()->second->getAngularSpeedConstant();
}

float BaseBot::getForwardSpeed() const
{
    return m_body->getForwardSpeed();
}

void BaseBot::setForwardSpeedCallback(std::function<void(float)> onForwardSpeedChanged)
{
    m_onForwardSpeedChanged = onForwardSpeedChanged;
}

void BaseBot::setTopSpeedCallback(std::function<void(float)> onTopSpeedChanged)
{
    m_onTopSpeedChanged = onTopSpeedChanged;
}

void BaseBot::setForwardAccelerationCallback(std::function<void(float)> onForwardAccelerationChanged)
{
    m_onForwardAccelerationChanged = onForwardAccelerationChanged;
}

void BaseBot::setTimeToTopSpeedCallback(std::function<void(float)> onTimeToTopSpeedChanged)
{
    m_onTimeToTopSpeedChanged = onTimeToTopSpeedChanged;
}

void BaseBot::setTopSpeedAccelerationCallback(std::function<void(float)> onTopSpeedAccelerationChanged)
{
    m_onTopSpeedAccelerationChanged = onTopSpeedAccelerationChanged;
}

void BaseBot::setTopAccelerationCallback(std::function<void(float)> onTopAccelerationChanged)
{
    m_onTopAccelerationChanged = onTopAccelerationChanged;
}

void BaseBot::setTimeMovingCallback(std::function<void(unsigned int)> onTimeMovingChanged)
{
    m_onTimeMovingChanged = onTimeMovingChanged;
}

glm::vec2 BaseBot::getAbsoluteWheelPosition(BaseBot::WheelMotorIndex wheelMotorIndex) const
{
    auto wheelMotorItr = m_wheelMotors.find(wheelMotorIndex);
    if (wheelMotorItr == m_wheelMotors.end()) {
        std::cout << "Wheel motor not found!" << std::endl;
        assert(0);
    }
    return wheelMotorItr->second->getBody()->getPosition();
}

void BaseBot::enableMotor(BaseBot::WheelMotorIndex wheelMotorIndex)
{
    auto wheelMotorItr = m_wheelMotors.find(wheelMotorIndex);
    if (wheelMotorItr == m_wheelMotors.end()) {
        std::cout << "Wheel motor not found!" << std::endl;
        assert(0);
    }
    wheelMotorItr->second->enable();
}

void BaseBot::disableMotor(BaseBot::WheelMotorIndex wheelMotorIndex)
{
    auto wheelMotorItr = m_wheelMotors.find(wheelMotorIndex);
    if (wheelMotorItr == m_wheelMotors.end()) {
        std::cout << "Wheel motor not found!" << std::endl;
        assert(0);
    }
    wheelMotorItr->second->disable();
}

float BaseBot::getAngularDamping() const
{
    return m_body->getAngularDamping();
}

void BaseBot::setAngularDamping(float angularDamping)
{
    m_body->setAngularDamping(angularDamping);
}
