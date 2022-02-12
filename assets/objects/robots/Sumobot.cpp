#include "robots/Sumobot.h"

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
            0.15f,          /* Coloumb friction coefficient */
            25.0f,        /* Wheel sideway friction constant */
            0.016f,      /* Motor voltage in constant */
            0.012f,      /* Angular speed constant */
            6.0f,          /* Motor max voltage */
            40.0f,      /* Body angular damping */
            SimpleBotBody::Shape::Rectangle,
            SimpleBotBody::TextureType::SumobotPlated,
            WheelMotor::TextureType::Orange,
            {
                { Sumobot::WheelMotorIndex::FrontLeft,  {-(0.07f + 0.015f) / 2,  0.1f / 5} },
                { Sumobot::WheelMotorIndex::FrontRight, { (0.07f + 0.015f) / 2,  0.1f / 5} },
                { Sumobot::WheelMotorIndex::BackLeft,   {-(0.07f + 0.015f) / 2, -0.1f / 4} },
                { Sumobot::WheelMotorIndex::BackRight,  { (0.07f + 0.015f) / 2, -0.1f / 4} },
            },
            {
                { Sumobot::RangeSensorIndex::Left,       {-0.035f, 0.00f}, { 4.71f, 0.0f, 0.8f, 0.03f }},
                { Sumobot::RangeSensorIndex::FrontLeft,  {-0.025f, 0.05f}, { 6.08f, 0.0f, 0.8f, 0.03f }},
                { Sumobot::RangeSensorIndex::Front,      { 0.000f, 0.05f}, { 0.0f, 0.0f, 0.8f, 0.03f }},
                { Sumobot::RangeSensorIndex::FrontRight, { 0.025f, 0.05f}, { 0.20f, 0.0f, 0.8f, 0.03f }},
                { Sumobot::RangeSensorIndex::Right,      { 0.035f, 0.00f}, { 1.57f, 0.0f, 0.8f, 0.03f }},
            },
            {
                { Sumobot::LineDetectorIndex::FrontLeft,  {-0.035f,  0.05f}, { 0.01f } },
                { Sumobot::LineDetectorIndex::FrontRight, { 0.035f,  0.05f}, { 0.01f } },
                { Sumobot::LineDetectorIndex::BackLeft,   {-0.035f, -0.05f}, { 0.01f } },
                { Sumobot::LineDetectorIndex::BackRight,  { 0.035f, -0.05f}, { 0.01f } },
            }
        }
    },
    { Sumobot::Blueprint::FourWheel,
        {
            0.07f, 0.1f,   /* Body width, length */
            0.42f,         /* Body mass */
            0.015f, 0.03f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            0.2f,          /* Coloumb friction coefficient */
            25.0f,        /* Wheel sideway friction constant */
            0.019f,      /* Motor voltage in constant */
            0.006f,      /* Angular speed constant */
            6.0f,          /* Motor max voltage */
            500.0f,      /* Body angular damping */
            SimpleBotBody::Shape::Rectangle,
            SimpleBotBody::TextureType::SumobotCircuited,
            WheelMotor::TextureType::Green,
            {
                { Sumobot::WheelMotorIndex::FrontLeft,  {-(0.07f + 0.015f) / 2,  0.1f / 5} },
                { Sumobot::WheelMotorIndex::FrontRight, { (0.07f + 0.015f) / 2,  0.1f / 5} },
                { Sumobot::WheelMotorIndex::BackLeft,   {-(0.07f + 0.015f) / 2, -0.1f / 4} },
                { Sumobot::WheelMotorIndex::BackRight,  { (0.07f + 0.015f) / 2, -0.1f / 4} },
            },
            {
                { Sumobot::RangeSensorIndex::Left,       {-0.035f, 0.00f}, { 4.71f, 0.0f, 0.8f, 0.0f }},
                { Sumobot::RangeSensorIndex::FrontLeft,  {-0.025f, 0.05f}, { 6.08f, 0.0f, 0.8f, 0.0f }},
                { Sumobot::RangeSensorIndex::Front,      { 0.000f, 0.05f}, { 0.0f, 0.0f, 0.8f, 0.0f }},
                { Sumobot::RangeSensorIndex::FrontRight, { 0.025f, 0.05f}, { 0.20f, 0.0f, 0.8f, 0.0f }},
                { Sumobot::RangeSensorIndex::Right,      { 0.035f, 0.00f}, { 1.57f, 0.0f, 0.8f, 0.0f }},
            },
            {
                { Sumobot::LineDetectorIndex::FrontLeft,  {-0.035f,  0.05f}, {0.0f } },
                { Sumobot::LineDetectorIndex::FrontRight, { 0.035f,  0.05f}, {0.0f } },
                { Sumobot::LineDetectorIndex::BackLeft,   {-0.035f, -0.05f}, {0.0f } },
                { Sumobot::LineDetectorIndex::BackRight,  { 0.035f, -0.05f}, {0.0f } },
            }
        }
    },
    { Sumobot::Blueprint::TwoWheelRectangle,
        {
            0.06f, 0.08f,   /* Body width, length */
            0.4f,         /* Body mass */
            0.02f, 0.04f, /* Wheel width, diameter */
            0.05f,         /* Wheel mass */
            0.2f,          /* Coloumb friction coefficient */
            50.0f,        /* Wheel sideway friction constant */
            0.00628f,      /* Motor voltage in constant */
            0.00178f,      /* Angular speed constant */
            6.0f,          /* Motor max voltage */
            0.0f,      /* Body angular damping */
            SimpleBotBody::Shape::Rectangle,
            SimpleBotBody::TextureType::SumobotPlated,
            WheelMotor::TextureType::Orange,
            {
                { Sumobot::WheelMotorIndex::Left,   {-(0.06f + 0.02f) / 2, -0.1f / 5} },
                { Sumobot::WheelMotorIndex::Right,  { (0.06f + 0.02f) / 2, -0.1f / 5} },
            },
            {
                { Sumobot::RangeSensorIndex::Left,       {-0.03f, 0.01f}, { 4.71f, 0.0f, 0.8f, 0.0f}},
                { Sumobot::RangeSensorIndex::FrontLeft,  {-0.025f, 0.03f}, { 6.08f, 0.0f, 0.8f, 0.0f }},
                { Sumobot::RangeSensorIndex::Front,      { 0.000f, 0.03f}, { 0.0f, 0.0f, 0.8f, 0.0f }},
                { Sumobot::RangeSensorIndex::FrontRight, { 0.025f, 0.03f}, { 0.20f, 0.0f, 0.8f, 0.0f }},
                { Sumobot::RangeSensorIndex::Right,      { 0.03f, 0.01f}, { 1.57f, 0.0f, 0.8f, 0.0f }},
            },
            {
                { Sumobot::LineDetectorIndex::FrontLeft,  {-0.029f,  0.04f}, {0.0f} },
                { Sumobot::LineDetectorIndex::FrontRight, { 0.029f,  0.04f}, {0.0f} },
                { Sumobot::LineDetectorIndex::BackLeft,   {-0.029f, -0.04f}, {0.0f} },
                { Sumobot::LineDetectorIndex::BackRight,  { 0.029f, -0.04f}, {0.0f} },
            }
        }
    },
    { Sumobot::Blueprint::TwoWheelRoundBlack,
        {
            0.075f, 0.08f,   /* Body width, length */
            0.42f,         /* Body mass */
            0.0125f, 0.0225f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            0.1f,          /* Coloumb friction coefficient */
            100.0f,        /* Wheel sideway friction constant */
            0.00500f,      /* Motor voltage in constant */
            0.00178f,      /* Angular speed constant */
            6.0f,          /* Motor max voltage */
            0.0f,      /* Body angular damping */
            SimpleBotBody::Shape::Circle,
            SimpleBotBody::TextureType::SumobotRoundBlack,
            WheelMotor::TextureType::Red,
            {
                { Sumobot::WheelMotorIndex::Left,  {-(0.075f + 0.0125f) / 2,  0.0f} },
                { Sumobot::WheelMotorIndex::Right, { (0.075f + 0.0125f) / 2,  0.0f} },
            },
            {
                { Sumobot::RangeSensorIndex::FrontLeft,  {-0.025f, 0.03f}, { 6.08f, 0.0f, 0.8f, 0.0f }},
                { Sumobot::RangeSensorIndex::Front,      { 0.000f, 0.04f}, { 0.0f, 0.0f, 0.8f, 0.0f }},
                { Sumobot::RangeSensorIndex::FrontRight, { 0.025f, 0.03f}, { 0.20f, 0.0f, 0.8f, 0.0f }},
            },
            {
                { Sumobot::LineDetectorIndex::FrontLeft,  {-0.0275f,  0.0275f }, {0.0f} },
                { Sumobot::LineDetectorIndex::FrontRight, { 0.0275f,  0.0275f }, {0.0f} },
                { Sumobot::LineDetectorIndex::BackLeft,   {-0.0275f, -0.0275f }, {0.0f} },
                { Sumobot::LineDetectorIndex::BackRight,  { 0.0275f, -0.0275f }, {0.0f} },
            }
        }
    },
    { Sumobot::Blueprint::TwoWheelRoundRed,
        {
            0.075f, 0.08f,   /* Body width, length */
            0.42f,         /* Body mass */
            0.0125f, 0.0225f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            0.1f,          /* Coloumb friction coefficient */
            100.0f,        /* Wheel sideway friction constant */
            0.00500f,      /* Motor voltage in constant */
            0.00178f,      /* Angular speed constant */
            6.0f,          /* Motor max voltage */
            0.0f,      /* Body angular damping */
            SimpleBotBody::Shape::Circle,
            SimpleBotBody::TextureType::SumobotRoundRed,
            WheelMotor::TextureType::Green,
            {
                { Sumobot::WheelMotorIndex::Left,  {-(0.075f + 0.0125f) / 2,  0.0f} },
                { Sumobot::WheelMotorIndex::Right, { (0.075f + 0.0125f) / 2,  0.0f} },
            },
            {
                { Sumobot::RangeSensorIndex::FrontLeft,  {-0.025f, 0.03f}, { 6.08f, 0.0f, 0.8f, 0.0f }},
                { Sumobot::RangeSensorIndex::FrontRight, { 0.025f, 0.03f}, { 0.20f, 0.0f, 0.8f, 0.0f }},
            },
            {
                { Sumobot::LineDetectorIndex::FrontLeft,  {-0.0275f,  0.0275f }, { 0.0f } },
                { Sumobot::LineDetectorIndex::FrontRight, { 0.0275f,  0.0275f }, { 0.0f } },
                { Sumobot::LineDetectorIndex::BackLeft,   {-0.0275f, -0.0275f }, { 0.0f } },
                { Sumobot::LineDetectorIndex::BackRight,  { 0.0275f, -0.0275f }, { 0.0f } },
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
