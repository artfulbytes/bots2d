#include "robots/Sumobot.h"
#include "Scene.h"

#include <iostream>

namespace {
const std::unordered_map<Sumobot::Blueprint, Sumobot::Specification> sumobotBlueprints
({
    { Sumobot::Blueprint::FourWheel,
        {
            0.07f, 0.1f,   /* Body width, length */
            0.42f,         /* Body mass */
            0.6f,          /* Body torque friction coefficient */
            0.015f, 0.03f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            100.0f,        /* Wheel sideway friction constant */
            0.00528f,      /* Motor voltage in constant */
            0.00178f,      /* Angular speed constant */
            6.0f,          /* Motor max voltage */
            GenericBody::Shape::Rectangle,
            GenericBody::TextureType::SumobotCircuited,
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
            0.3f,         /* Body torque friction coefficient */
            0.02f, 0.04f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            150.0f,        /* Wheel sideway friction constant */
            0.00628f,      /* Motor voltage in constant */
            0.00178f,      /* Angular speed constant */
            6.0f,          /* Motor max voltage */
            GenericBody::Shape::Rectangle,
            GenericBody::TextureType::SumobotPlated,
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
            0.35f,         /* Body torque friction coefficient */
            0.0125f, 0.0225f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            100.0f,        /* Wheel sideway friction constant */
            0.00500f,      /* Motor voltage in constant */
            0.00178f,      /* Angular speed constant */
            6.0f,          /* Motor max voltage */
            GenericBody::Shape::Circle,
            GenericBody::TextureType::SumobotRoundBlack,
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
            0.35f,         /* Body torque friction coefficient */
            0.0125f, 0.0225f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            100.0f,        /* Wheel sideway friction constant */
            0.00500f,      /* Motor voltage in constant */
            0.00178f,      /* Angular speed constant */
            6.0f,          /* Motor max voltage */
            GenericBody::Shape::Circle,
            GenericBody::TextureType::SumobotRoundRed,
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
    },
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

Sumobot::Sumobot(Scene *scene, const Sumobot::Specification &spec,
                 const glm::vec2 &startPosition, const float startRotation) :
    BaseBot(scene, spec, startPosition, startRotation)
{
}

Sumobot::~Sumobot()
{
}
