#include "robots/Sumobot.h"

#include "Scene.h"
#include "sensors/LineDetectorObject.h"
#include "components/KeyboardController.h"

#include <iostream>

namespace {
const std::unordered_map<Sumobot::Blueprint, Sumobot::Specification> sumobotBlueprints
({
    { Sumobot::Blueprint::Nsumo,
        {
            0.07f, 0.1f,   /* Body width, length */
            0.42f,         /* Body mass */
            0.015f, 0.03f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            SumobotBody::Shape::Rectangle,
            SumobotBody::TextureType::Circuited,
            WheelMotor::TextureType::Green,
            {
                { Sumobot::WheelMotorIndex::FrontLeft,  {-(0.07f + 0.015f) / 2,  0.1f / 4} },
                { Sumobot::WheelMotorIndex::FrontRight, { (0.07f + 0.015f) / 2,  0.1f / 4} },
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

void Sumobot::createBody(const Sumobot::Specification &spec, const glm::vec2 &startPosition, const float startRotation)
{
    const SumobotBody::Specification bodySpec(spec.bodyLength, spec.bodyWidth, spec.bodyMass,
                                              spec.bodyShape, spec.bodyTexture);
    m_sumobotBody = std::make_unique<SumobotBody>(m_scene, bodySpec, startPosition);
}

void Sumobot::createWheelMotors(const Sumobot::Specification &spec)
{
    const WheelMotor::Specification wheelSpec(spec.wheelWidth, spec.wheelDiameter,
                                              spec.wheelMass, spec.wheelTexture);
    for (const auto &wheelMotorTuple : spec.wheelMotorTuples) {
        const auto index = std::get<0>(wheelMotorTuple);
        const auto position = std::get<1>(wheelMotorTuple);
        switch (index) {
        case Sumobot::WheelMotorIndex::Left:
        case Sumobot::WheelMotorIndex::FrontLeft:
        case Sumobot::WheelMotorIndex::BackLeft:
            m_wheelMotors[index] = std::make_unique<WheelMotor>(m_scene, wheelSpec, WheelMotor::Orientation::Left, position);
            break;
        case Sumobot::WheelMotorIndex::Right:
        case Sumobot::WheelMotorIndex::FrontRight:
        case Sumobot::WheelMotorIndex::BackRight:
            m_wheelMotors[index] = std::make_unique<WheelMotor>(m_scene, wheelSpec, WheelMotor::Orientation::Right, position);
            break;
        }
        m_sumobotBody->attachWheelMotor(m_wheelMotors[index].get(), position);
    }
}

void Sumobot::createSensors(const Sumobot::Specification &spec)
{
    for (const auto &rangeSensorTuple : spec.rangeSensorTuples) {
        const auto index = std::get<0>(rangeSensorTuple);
        const auto position = std::get<1>(rangeSensorTuple);
        const auto spec = std::get<2>(rangeSensorTuple);
        m_rangeSensors[index] = std::make_unique<RangeSensorObject>(m_scene, spec, true, position);
        m_sumobotBody->attachSensor(m_rangeSensors[index].get(), position);
    }
    for (const auto &lineDetectorTuple : spec.lineDetectorTuples) {
        const auto index = std::get<0>(lineDetectorTuple);
        const auto position = std::get<1>(lineDetectorTuple);
        m_lineDetectors[index] = std::make_unique<LineDetectorObject>(m_scene, true, position);
        m_sumobotBody->attachSensor(m_lineDetectors[index].get(), position);
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
