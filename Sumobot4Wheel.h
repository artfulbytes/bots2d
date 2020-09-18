#ifndef SUMOBOT_H_
#define SUMOBOT_H_

#include <vector>

// TODO: Can't use <> why?
#include "Wheel.h"

class Sumobot4Wheel
{
public:
    // TODO: Why enum class over regular enum?
    // TODO: How to name enums when there is an inner enum doing the same thing
    // TODO: All caps?
    enum class Drive {Forward, Backward, None};
    enum class Turn {Left, Right, None};

    Sumobot4Wheel(b2World* world, float width, float length, float weight);
    void update(Drive drive, Turn turn);

private:
    void createBody(b2World* world);
    // TODO: Save ptr to the world inside this object?
    void createWheels(b2World* world); 
    // TODO: Must forward declare here, ugly...


    Wheel::Drive driveToWheelDrive(Drive drive);
    Wheel::Drive motionToWheelDrive(Drive drive, Turn turn, Wheel::Type wheelType);

    //Wheel::Turn sumobotToWheelTurn(Turn turn);

    // TODO: Initialize these here?
    b2Body* m_body;
    std::vector<Wheel*> m_wheels;
    b2RevoluteJoint *m_flJoint, *m_frJoint;

    float m_width = 0;
    float m_length = 0;
    float m_weight = 0;
    // TODO: Introduce height for density calcultion?

    // TODO: Is it okay to use const here? I can't use constexpr
    const float m_weightBodyFactor = 0.9f;
    const float m_weightWheelsFactor = 1.0f - m_weightBodyFactor;
    const float m_widthBodyFactor = 0.8f;
    const float m_widthWheelsFactor = 1.0f - m_widthBodyFactor;

    // TODO: These should be passed instead
    float m_maxForwardSpeed = 2; // m/s
    float m_maxBackwardSpeed = -2; // m/s
    float m_maxAcceleration = 1; // m/s^2

    //float m_backWheelMaxDriveForce = 300;
    //float m_frontWheelMaxDriveForce = 500;
    //float m_backWheelMaxLateralImpulse = 8.5;
    //float m_frontWheelMaxLateralImpulse = 7.5;
};

#endif /* SUMOBOT_H_ */
