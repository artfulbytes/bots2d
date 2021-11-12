#include "robots/LineFollower.h"

#include <iostream>

namespace {
const std::unordered_map<LineFollower::Blueprint, LineFollower::Specification> lineFollowerBlueprints
({
    { LineFollower::Blueprint::FourFrontSensors,
        {
            /* TODO: Make the center of mass more realistic */
            0.07f, 0.094f, /* Body width, length */
            0.22f,         /* Body mass */
            0.015f, 0.03f, /* Wheel width, diameter */
            0.02f,         /* Wheel mass */
            0.1f,          /* Coloumb friction coefficient */
            100.0f,        /* Wheel sideway friction constant */
            0.00628f,      /* Motor voltage in constant */
            0.00178f,      /* Angular speed constant */
            3.0f,          /* Motor max voltage */
            0.0f,        /* Angular damping */
            SimpleBotBody::Shape::Rectangle,
            SimpleBotBody::TextureType::LineFollowerPlated,
            WheelMotor::TextureType::Orange,
            {
                { LineFollower::WheelMotorIndex::Left,  {-(0.07f + 0.015f) / 2,  -0.027f } },
                { LineFollower::WheelMotorIndex::Right, { (0.07f + 0.015f) / 2,  -0.027f } },
            },
            {
                /* No range sensors */
            },
            {
                { LineFollower::LineDetectorIndex::SecondFrontLeft,  {-0.010f,  0.045f} },
                { LineFollower::LineDetectorIndex::FrontLeft,        {-0.005f,  0.045f} },
                { LineFollower::LineDetectorIndex::FrontRight,       { 0.005f,  0.045f} },
                { LineFollower::LineDetectorIndex::SecondFrontRight, { 0.010f,  0.045f} },
            }
        }
    },
});
}

const LineFollower::Specification &LineFollower::getBlueprintSpec(LineFollower::Blueprint blueprint)
{
    auto blueprintItr = lineFollowerBlueprints.find(blueprint);
    if (blueprintItr == lineFollowerBlueprints.end()) {
        std::cout << "Blueprint not found!" << std::endl;
        assert(0);
    }
    return blueprintItr->second;
}

LineFollower::LineFollower(Scene *scene, const LineFollower::Specification &spec,
                 const glm::vec2 &startPosition, const float startRotation) :
    BaseBot(scene, spec, startPosition, startRotation)
{
}

LineFollower::~LineFollower()
{
}
