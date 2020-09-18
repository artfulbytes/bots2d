#include "Sumobot4Wheel.h"
#include "Wheel.h"

// TODO: Move to separate file?, should I ever use defs in c++?
// TODO: pragma once?
// TODO: Same here, need "" not <>
#include "Constants.h"

using namespace constants;

// TODO: Initializer list like this?
Sumobot4Wheel::Sumobot4Wheel(b2World* world, float width, float length, float weight)
    : m_width(width)
    , m_length(length)
    , m_weight(weight)
{
    createBody(world);
    createWheels(world);
}

// TODO: What order to place private and public functiosn in?
//       Any name difference between the two?
Wheel::Drive Sumobot4Wheel::driveToWheelDrive(Drive drive) {
    switch (drive) {
    case Drive::Forward:
        return Wheel::Drive::Forward;
    case Drive::Backward:
        return Wheel::Drive::Backward;
    default:
        return Wheel::Drive::None;
    }
}

Wheel::Drive Sumobot4Wheel::motionToWheelDrive(Drive drive, Turn turn, Wheel::Type wheelType)
{
    if (drive == Drive::None) {
        return Wheel::Drive::None;
    }

    // TODO: Place this before or after the top if statement?
    Drive oppositeDrive = Drive::None;

    switch(drive) {
        case Drive::Forward:
            oppositeDrive = Drive::Backward;
            break;
        case Drive::Backward:
            oppositeDrive = Drive::Forward;
            break;
        default:
            oppositeDrive = Drive::None;
    }

    switch(turn)
    {
        case Turn::Left:
            switch(wheelType)
            {
                case Wheel::Type::Left: return driveToWheelDrive(oppositeDrive);
                case Wheel::Type::Right: return driveToWheelDrive(drive);
            }
        case Turn::Right:
            switch(wheelType)
            {
                case Wheel::Type::Left: return driveToWheelDrive(drive);
                case Wheel::Type::Right: return driveToWheelDrive(oppositeDrive);
            }
        default:
            return driveToWheelDrive(drive);
    }
}

/*
Wheel::Turn Sumobot4Wheel::sumobotToWheelTurn(Turn turn) {
    switch(turn) {
    case Turn::Left:  return Wheel::Turn::Left;
    case Turn::Right: return Wheel::Turn::Right;
    case Turn::None:  return Wheel::Turn::None;
    }
    return Wheel::Turn::None;
}
*/

void Sumobot4Wheel::update(Drive drive, Turn turn) {
    for (auto &wheel : m_wheels) {
        wheel->updateFriction();
    }
    for (auto &wheel : m_wheels) {
        wheel->updateDrive(motionToWheelDrive(drive, turn, wheel->getType()));
    }
    
    //control steering
    /*
    float lockAngle = 35 * DEGTORAD;
    float turnSpeedPerSec = 160 * DEGTORAD;//from lock to lock in 0.5 sec
    float turnPerTimeStep = turnSpeedPerSec / 60.0f;
    float desiredAngle = 0;
    switch (turn) {
    case Turn::Left:
        desiredAngle = lockAngle;
        break;
    case Turn::Right:
        desiredAngle = -lockAngle;
        break;
    case Turn::None:
        break;
    }
    */
    //float angleNow = m_flJoint->GetJointAngle();
    //float angleToTurn = desiredAngle - angleNow;
    //angleToTurn = b2Clamp(angleToTurn, -turnPerTimeStep, turnPerTimeStep);
    //float newAngle = angleNow + angleToTurn;
    //m_flJoint->SetLimits(newAngle, newAngle);
    //m_frJoint->SetLimits(newAngle, newAngle);
}

// TODO: Define m_width... or pass them as parameters?
void Sumobot4Wheel::createBody(b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);
    m_body->SetAngularDamping(3);

    const float scaledWidth = m_width * lengthScaleFactor * m_widthBodyFactor;
    const float scaledLength = m_length * lengthScaleFactor;
    const float scaledWeight = m_weight * weightScaleFactor * m_weightBodyFactor;
    const float density = scaledWeight / (scaledWidth * scaledLength);

    b2Vec2 vertices[4];
    vertices[0].Set(scaledWidth / 2, scaledLength / 2);
    vertices[1].Set(scaledWidth / 2, -scaledLength / 2);
    vertices[2].Set(-scaledWidth / 2, -scaledLength / 2);
    vertices[3].Set(-scaledWidth / 2, scaledLength / 2);
    b2PolygonShape polygonShape;
    polygonShape.Set(vertices, 4);

    m_body->CreateFixture(&polygonShape, density);
}

void Sumobot4Wheel::createWheels(b2World* world)
{
    const float scaledWheelWidth = (m_width * lengthScaleFactor * m_widthWheelsFactor) / 2.0f;
    const float scaledWheelLength = (m_length * lengthScaleFactor) / 4.0f;
    const float scaledWheelWeight = (m_weight * weightScaleFactor) / 4.0f;
    // TODO: FIx repeated code...
    const float scaledBodyWidth = m_width * lengthScaleFactor * m_widthBodyFactor;
    const float scaledBodyLength = m_length * lengthScaleFactor;

    const float frontRightX = (scaledBodyWidth / 2) + (scaledWheelWidth / 2);
    const float frontRightY = scaledBodyLength / 4;
    const float frontLeftX = -frontRightX;
    const float frontLeftY = frontRightY;
    const float backRightX = frontRightX;
    const float backRightY = -frontRightY;
    const float backLeftX = frontLeftX;
    const float backLeftY = -frontLeftY;

    // Prepare common joint parameters
    // TODO: Use a weld joint here instead
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = m_body;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorB.SetZero(); //center of wheel

    // TODO: F = m*a
    float maxTotalForce = m_maxAcceleration * m_weight;
    // TODO: 4 as number of wheels (maybe use int?)
    float maxForcePerWheel = maxTotalForce / 4.0f;
    float scaledMaxForcePerWheel = forceScaleFactor * maxForcePerWheel;
    float scaledMaxForwardSpeed = m_maxForwardSpeed * speedScaleFactor;
    float scaledMaxBackwardSpeed = m_maxForwardSpeed * speedScaleFactor;
    // TODO: Pass params as struct?

    // TODO: When to use AUTO?
    // TODO: When to use new?
    Wheel* backLeftWheel = new Wheel(world, Wheel::Type::Left, scaledWheelWidth,
                                     scaledWheelLength, scaledWheelWeight);
    backLeftWheel->setCharacteristics(scaledMaxForwardSpeed, scaledMaxBackwardSpeed, 
                                      scaledMaxForcePerWheel, 0);
    jointDef.bodyB = backLeftWheel->getBody();
    jointDef.localAnchorA.Set(backLeftX, backLeftY);
    world->CreateJoint(&jointDef);
    m_wheels.push_back(backLeftWheel);

    Wheel* backRightWheel = new Wheel(world, Wheel::Type::Right, scaledWheelWidth,
                                     scaledWheelLength, scaledWheelWeight);
    backRightWheel->setCharacteristics(scaledMaxForwardSpeed, scaledMaxBackwardSpeed, 
                                       scaledMaxForcePerWheel, 0);
    jointDef.bodyB = backRightWheel->getBody();
    jointDef.localAnchorA.Set(backRightX, backRightY);
    world->CreateJoint(&jointDef);
    m_wheels.push_back(backRightWheel);

    Wheel* frontLeftWheel = new Wheel(world, Wheel::Type::Left, scaledWheelWidth,
                                     scaledWheelLength, scaledWheelWeight);
    frontLeftWheel->setCharacteristics(scaledMaxForwardSpeed, scaledMaxBackwardSpeed, 
                                       scaledMaxForcePerWheel, 0);
    jointDef.bodyB = frontLeftWheel->getBody();
    jointDef.localAnchorA.Set(frontLeftX, frontLeftY);
    m_flJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
    m_wheels.push_back(frontLeftWheel);

    Wheel* frontRightWheel = new Wheel(world, Wheel::Type::Right, scaledWheelWidth,
                                     scaledWheelLength, scaledWheelWeight);
    frontRightWheel->setCharacteristics(scaledMaxForwardSpeed, scaledMaxBackwardSpeed, 
                                        scaledMaxForcePerWheel, 0);
    jointDef.bodyB = frontRightWheel->getBody();
    jointDef.localAnchorA.Set(frontRightX, frontRightY);
    m_frJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
    m_wheels.push_back(frontRightWheel);
}
