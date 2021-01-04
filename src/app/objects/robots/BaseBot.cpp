#include "robots/BaseBot.h"

#include "Scene.h"
#include "sensors/LineDetectorObject.h"
#include "components/KeyboardController.h"
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
    const GenericBody::Specification bodySpec(spec.bodyLength, spec.bodyWidth, spec.bodyMass,
                                              spec.bodyTorqueFrictionCoefficient, spec.bodyShape, spec.bodyTexture);
    m_body = std::make_unique<GenericBody>(m_scene, bodySpec, startPosition, startRotation);
}

void BaseBot::createWheelMotors(const BaseBot::Specification &spec)
{
    const WheelMotor::Specification wheelSpec(spec.motorVoltageInConstant, spec.motorAngularSpeedConstant,
                                              spec.motorMaxVoltage, spec.wheelSidewayFrictionConstant,
                                              spec.wheelWidth, spec.wheelDiameter, spec.wheelMass, spec.wheelTexture);
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

void BaseBot::onFixedUpdate(float stepTime)
{
    (void)stepTime;
}
