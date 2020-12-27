#include "robots/Sumobot.h"

#include "Scene.h"
#include "sensors/LineDetectorObject.h"
#include "components/KeyboardController.h"
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

namespace {
const std::unordered_map<Sumobot::Blueprint, Sumobot::Specification> sumobotBlueprints
({
    { Sumobot::Blueprint::FourWheel,
        {
            0.07f, 0.1f,   /* Body width, length */
            0.42f,         /* Body mass */
            0.015f, 0.03f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            SumobotBody::Shape::Rectangle,
            SumobotBody::TextureType::Circuited,
            WheelMotor::TextureType::Green,
            {
                { Sumobot::WheelMotorIndex::FrontLeft,  {-(0.07f + 0.015f) / 2,  0.1f / 5} },
                { Sumobot::WheelMotorIndex::FrontRight, { (0.07f + 0.015f) / 2,  0.1f / 5} },
                { Sumobot::WheelMotorIndex::BackLeft,   {-(0.07f + 0.015f) / 2, -0.1f / 4} },
                { Sumobot::WheelMotorIndex::BackRight,  { (0.07f + 0.015f) / 2, -0.1f / 4} },
            },
            {
                { Sumobot::RangeSensorIndex::Left,       {-0.035f, 0.00f}, { 4.71f, 0.0f, 0.8f }},
                { Sumobot::RangeSensorIndex::FrontLeft,  {-0.025f, 0.05f}, { 6.08f, 0.0f, 0.8f }},
                { Sumobot::RangeSensorIndex::Front,      { 0.000f, 0.05f}, { 0.0f, 0.0f, 0.8f }},
                { Sumobot::RangeSensorIndex::FrontRight, { 0.025f, 0.05f}, { 0.20f, 0.0f, 0.8f }},
                { Sumobot::RangeSensorIndex::Right,      { 0.035f, 0.00f}, { 1.57f, 0.0f, 0.8f }},
            },
            {
                { Sumobot::LineDetectorIndex::FrontLeft,  {-0.035f,  0.05f} },
                { Sumobot::LineDetectorIndex::FrontRight, { 0.035f,  0.05f} },
                { Sumobot::LineDetectorIndex::BackLeft,   {-0.035f, -0.05f} },
                { Sumobot::LineDetectorIndex::BackRight,  { 0.035f, -0.05f} },
            }
        }
    },
    { Sumobot::Blueprint::TwoWheelRectangle,
        {
            0.06f, 0.08f,   /* Body width, length */
            0.25f,         /* Body mass */
            0.02f, 0.04f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            SumobotBody::Shape::Rectangle,
            SumobotBody::TextureType::Plated,
            WheelMotor::TextureType::Orange,
            {
                { Sumobot::WheelMotorIndex::Left,   {-(0.06f + 0.02f) / 2, -0.1f / 5} },
                { Sumobot::WheelMotorIndex::Right,  { (0.06f + 0.02f) / 2, -0.1f / 5} },
            },
            {
                { Sumobot::RangeSensorIndex::Left,       {-0.03f, 0.01f}, { 4.71f, 0.0f, 0.8f }},
                { Sumobot::RangeSensorIndex::FrontLeft,  {-0.025f, 0.03f}, { 6.08f, 0.0f, 0.8f }},
                { Sumobot::RangeSensorIndex::Front,      { 0.000f, 0.03f}, { 0.0f, 0.0f, 0.8f }},
                { Sumobot::RangeSensorIndex::FrontRight, { 0.025f, 0.03f}, { 0.20f, 0.0f, 0.8f }},
                { Sumobot::RangeSensorIndex::Right,      { 0.03f, 0.01f}, { 1.57f, 0.0f, 0.8f }},
            },
            {
                { Sumobot::LineDetectorIndex::FrontLeft,  {-0.029f,  0.04f} },
                { Sumobot::LineDetectorIndex::FrontRight, { 0.029f,  0.04f} },
                { Sumobot::LineDetectorIndex::BackLeft,   {-0.029f, -0.04f} },
                { Sumobot::LineDetectorIndex::BackRight,  { 0.029f, -0.04f} },
            }
        }
    },
    { Sumobot::Blueprint::TwoWheelRoundBlack,
        {
            0.075f, 0.08f,   /* Body width, length */
            0.42f,         /* Body mass */
            0.0125f, 0.0225f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            SumobotBody::Shape::Circle,
            SumobotBody::TextureType::RoundBlack,
            WheelMotor::TextureType::Red,
            {
                { Sumobot::WheelMotorIndex::Left,  {-(0.075f + 0.0125f) / 2,  0.0f} },
                { Sumobot::WheelMotorIndex::Right, { (0.075f + 0.0125f) / 2,  0.0f} },
            },
            {
                { Sumobot::RangeSensorIndex::FrontLeft,  {-0.025f, 0.03f}, { 6.08f, 0.0f, 0.8f }},
                { Sumobot::RangeSensorIndex::Front,      { 0.000f, 0.04f}, { 0.0f, 0.0f, 0.8f }},
                { Sumobot::RangeSensorIndex::FrontRight, { 0.025f, 0.03f}, { 0.20f, 0.0f, 0.8f }},
            },
            {
                { Sumobot::LineDetectorIndex::FrontLeft,  {-0.0275f,  0.0275f} },
                { Sumobot::LineDetectorIndex::FrontRight, { 0.0275f,  0.0275f} },
                { Sumobot::LineDetectorIndex::BackLeft,   {-0.0275f, -0.0275f} },
                { Sumobot::LineDetectorIndex::BackRight,  { 0.0275f, -0.0275f} },
            }
        }
    },
    { Sumobot::Blueprint::TwoWheelRoundRed,
        {
            0.075f, 0.08f,   /* Body width, length */
            0.42f,         /* Body mass */
            0.0125f, 0.0225f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            SumobotBody::Shape::Circle,
            SumobotBody::TextureType::RoundRed,
            WheelMotor::TextureType::Green,
            {
                { Sumobot::WheelMotorIndex::Left,  {-(0.075f + 0.0125f) / 2,  0.0f} },
                { Sumobot::WheelMotorIndex::Right, { (0.075f + 0.0125f) / 2,  0.0f} },
            },
            {
                { Sumobot::RangeSensorIndex::FrontLeft,  {-0.025f, 0.03f}, { 6.08f, 0.0f, 0.8f }},
                { Sumobot::RangeSensorIndex::FrontRight, { 0.025f, 0.03f}, { 0.20f, 0.0f, 0.8f }},
            },
            {
                { Sumobot::LineDetectorIndex::FrontLeft,  {-0.0275f,  0.0275f} },
                { Sumobot::LineDetectorIndex::FrontRight, { 0.0275f,  0.0275f} },
                { Sumobot::LineDetectorIndex::BackLeft,   {-0.0275f, -0.0275f} },
                { Sumobot::LineDetectorIndex::BackRight,  { 0.0275f, -0.0275f} },
            }
        }
    }
});
}

const Sumobot::Specification &Sumobot::getBlueprintSpec(Sumobot::Blueprint blueprint)
{
    auto blueprintItr = sumobotBlueprints.find(blueprint);
    if (blueprintItr == sumobotBlueprints.end()) {
        std::cout << "Blueprint not found!" << std::endl;
        assert(0);
    }
    return blueprintItr->second;
}

void Sumobot::sanityCheckSpec(const Sumobot::Specification &spec)
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

Sumobot::Sumobot(Scene *scene, const Sumobot::Specification &spec,
                 const glm::vec2 &startPosition, const float startRotation) :
    SceneObject(scene)
{
    sanityCheckSpec(spec);
    createBody(spec, startPosition, startRotation);
    createWheelMotors(spec);
    createSensors(spec);
}

Sumobot::~Sumobot()
{
}

void Sumobot::createBody(const Sumobot::Specification &spec, const glm::vec2 &startPosition, float startRotation)
{
    const SumobotBody::Specification bodySpec(spec.bodyLength, spec.bodyWidth, spec.bodyMass,
                                              spec.bodyShape, spec.bodyTexture);
    m_sumobotBody = std::make_unique<SumobotBody>(m_scene, bodySpec, startPosition, startRotation);
}

void Sumobot::createWheelMotors(const Sumobot::Specification &spec)
{
    const WheelMotor::Specification wheelSpec(spec.wheelWidth, spec.wheelDiameter,
                                              spec.wheelMass, spec.wheelTexture);
    for (const auto &wheelMotorTuple : spec.wheelMotorTuples) {
        const auto index = std::get<0>(wheelMotorTuple);
        const auto relativePosition = std::get<1>(wheelMotorTuple);
        const auto rotation = m_sumobotBody->getRotation();
        const auto absolutePosition = m_sumobotBody->getPosition() + glm::rotate(relativePosition, rotation);
        switch (index) {
        case Sumobot::WheelMotorIndex::Left:
        case Sumobot::WheelMotorIndex::FrontLeft:
        case Sumobot::WheelMotorIndex::BackLeft:
            m_wheelMotors[index] = std::make_unique<WheelMotor>(m_scene, wheelSpec, WheelMotor::Orientation::Left,
                                                                absolutePosition, rotation);
            break;
        case Sumobot::WheelMotorIndex::Right:
        case Sumobot::WheelMotorIndex::FrontRight:
        case Sumobot::WheelMotorIndex::BackRight:
            m_wheelMotors[index] = std::make_unique<WheelMotor>(m_scene, wheelSpec, WheelMotor::Orientation::Right,
                                                                absolutePosition, rotation);
            break;
        }
        m_sumobotBody->attachWheelMotor(m_wheelMotors[index].get(), relativePosition);
    }
}

void Sumobot::createSensors(const Sumobot::Specification &spec)
{
    for (const auto &rangeSensorTuple : spec.rangeSensorTuples) {
        const auto index = std::get<0>(rangeSensorTuple);
        const auto relativePosition = std::get<1>(rangeSensorTuple);
        const auto absolutePosition = m_sumobotBody->getPosition() + glm::rotate(relativePosition, m_sumobotBody->getRotation());
        const auto spec = std::get<2>(rangeSensorTuple);
        m_rangeSensors[index] = std::make_unique<RangeSensorObject>(m_scene, spec, false, absolutePosition);
        m_sumobotBody->attachSensor(m_rangeSensors[index].get(), relativePosition);
    }
    for (const auto &lineDetectorTuple : spec.lineDetectorTuples) {
        const auto index = std::get<0>(lineDetectorTuple);
        const auto relativePosition = std::get<1>(lineDetectorTuple);
        const auto absolutePosition = m_sumobotBody->getPosition() + glm::rotate(relativePosition, m_sumobotBody->getRotation());
        m_lineDetectors[index] = std::make_unique<LineDetectorObject>(m_scene, false, absolutePosition);
        m_sumobotBody->attachSensor(m_lineDetectors[index].get(), relativePosition);
    }
}

void Sumobot::setDebug(bool enabled)
{
    for (const auto &lineDetectorTuple : m_lineDetectors) {
        lineDetectorTuple.second->setDebugDraw(enabled);
    }
    for (const auto &rangeSensorTuple : m_rangeSensors) {
        rangeSensorTuple.second->setDebugDraw(enabled);
    }
}

float *Sumobot::getVoltageLine(Sumobot::WheelMotorIndex wheelMotorIndex) const
{
    auto wheelMotorItr = m_wheelMotors.find(wheelMotorIndex);
    if (wheelMotorItr == m_wheelMotors.end()) {
        std::cout << "Wheel motor not found!" << std::endl;
        assert(0);
    }
    return wheelMotorItr->second->getVoltageLine();
}
