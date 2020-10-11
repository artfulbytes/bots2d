#include "Sumobot4Wheel.h"
#include "WheelMotor.h"
#include <iostream>

// TODO: Move to separate file?, should I ever use defs in c++?
// TODO: pragma once?
// TODO: Same here, need "" not <>
#include "Constants.h"
#include "draw.h"
#include "UserData.h"

#include "LineDetector.h"

// TODO: Proper cleanup of bodies and fixtures

using namespace constants;

// TODO: Initializer list like this?
Sumobot4Wheel::Sumobot4Wheel(b2World* world, float width, float length, float mass) :
    m_width(width),
    m_length(length),
    m_mass(mass)
{
    createBody(world);
    createFrictionBody(world);
    createWheels(world);
    createSensors(world);
}

void Sumobot4Wheel::createSensors(b2World* world)
{
    // TODO: new here or create empty range sensor by default in header?
    m_rangeSensorLeft = new RangeSensor(world, m_body, b2Vec2(0.0f, 0.4f), -pi / 2, 0.0f, 15.0f);
    m_rangeSensorFrontLeft = new RangeSensor(world, m_body, b2Vec2(0.15f, 0.52f), 0.10f, 0.0f, 15.0f);
    m_rangeSensorFront = new RangeSensor(world, m_body, b2Vec2(0.0f, 0.52f), 0.0f, 0.0f, 15.0f);
    m_rangeSensorFrontRight = new RangeSensor(world, m_body, b2Vec2(-0.15f, 0.52f), -0.10f, 0.0f, 15.0f);
    m_rangeSensorRight = new RangeSensor(world, m_body, b2Vec2(0.0f, 0.4f), pi / 2, 0.0f, 15.0f);

    m_lineDetectorFrontLeft = new LineDetector(world, m_body, b2Vec2(-0.35f, 0.45f));
    m_lineDetectorFrontRight = new LineDetector(world, m_body, b2Vec2(0.35f, 0.45f));
    m_lineDetectorBackRight = new LineDetector(world, m_body, b2Vec2(0.35f, -0.45f));
    m_lineDetectorBackLeft = new LineDetector(world, m_body, b2Vec2(-0.35f, -0.45f));
}

// TODO: Define m_width... or pass them as parameters?
void Sumobot4Wheel::createBody(b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    const float scaledWidth = m_width * lengthScaleFactor * m_widthBodyFactor;
    const float scaledLength = m_length * lengthScaleFactor;
    const float scaledMass = m_mass * massScaleFactor * m_massBodyFactor;
    const float density = scaledMass / (scaledWidth * scaledLength);

    b2Vec2 vertices[4];
    vertices[0].Set(scaledWidth / 2, scaledLength / 2);
    vertices[1].Set(scaledWidth / 2, -scaledLength / 2);
    vertices[2].Set(-scaledWidth / 2, -scaledLength / 2);
    vertices[3].Set(-scaledWidth / 2, scaledLength / 2);
    b2PolygonShape polygonShape;
    polygonShape.Set(vertices, 4);

    m_body->CreateFixture(&polygonShape, density);
}

void Sumobot4Wheel::createFrictionBody(b2World* world)
{
    b2BodyDef frictionBodyDef;
    m_frictionBody = world->CreateBody(&frictionBodyDef);
    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = true;
    // Since the fixture is just an "invisible" sensor the
    // exact shape doesn't matter, but it must have some shape.
    b2CircleShape circleShape;
    circleShape.m_radius = 0.0f;
    fixtureDef.shape = &circleShape;
    // TODO: Keep pointer to fixture?
    m_frictionBody->CreateFixture(&fixtureDef);
}

// TODO: What order to place private and public functiosn in?
//       Any name difference between the two?
WheelMotor::Drive Sumobot4Wheel::driveToWheelDrive(Drive drive) {
    switch (drive) {
    case Drive::Forward:
        return WheelMotor::Drive::Forward;
    case Drive::Backward:
        return WheelMotor::Drive::Backward;
    default:
        return WheelMotor::Drive::None;
    }
}

WheelMotor::Drive Sumobot4Wheel::motionToWheelDrive(Drive drive, Turn turn, WheelMotor::Type wheelType)
{
    if (drive == Drive::None) {
        return WheelMotor::Drive::None;
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
                case WheelMotor::Type::Left: return driveToWheelDrive(oppositeDrive);
                case WheelMotor::Type::Right: return driveToWheelDrive(drive);
            }
        case Turn::Right:
            switch(wheelType)
            {
                case WheelMotor::Type::Left: return driveToWheelDrive(drive);
                case WheelMotor::Type::Right: return driveToWheelDrive(oppositeDrive);
            }
        default:
            return driveToWheelDrive(drive);
    }
}

void Sumobot4Wheel::update(Drive drive, Turn turn) {
    for (auto &wheel : m_wheels) {
        wheel->updateFriction(motionToWheelDrive(drive, turn, wheel->getType()));
    }
    for (auto &wheel : m_wheels) {
        // TODO: Currently just applying 6 V
        wheel->updateDrive(motionToWheelDrive(drive, turn, wheel->getType()), 3.0f);
    }

    b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0,1));
    float currentSpeed = b2Dot(currentForwardNormal, m_body->GetLinearVelocity());
    //TODO: Add angular friction like this or some other way?
    float angularDamping = 1.0f; // Tweaked
    m_body->ApplyAngularImpulse(angularDamping * m_body->GetInertia() * -m_body->GetAngularVelocity(), true);

    updateSensors();
}

void Sumobot4Wheel::addFriction(b2World* world, b2Body* body, float maxFrictionForce)
{
    b2FrictionJointDef jointDef;
    jointDef.bodyA = m_frictionBody;
    jointDef.bodyB = body;
    // TODO: This should scale according to how big the normal force
    // from the ground is (i.e. how heavy the vehicle is)
    jointDef.maxForce = maxFrictionForce;
    world->CreateJoint(&jointDef);
}

void Sumobot4Wheel::createWheels(b2World* world)
{
    // TODO: Scale the member variables at initialization instead?
    const float scaledWheelWidth = (m_width * lengthScaleFactor * m_widthWheelsFactor) / 2.0f;
    const float scaledWheelRadius = ((m_length / 2.0f) * lengthScaleFactor) / 4.0f;
    const float scaledWheelWeight = (m_mass * massScaleFactor * m_massWheelsFactor) / m_numWheels;
    const float scaledBodyWidth = m_width * lengthScaleFactor * m_widthBodyFactor;
    const float scaledBodyLength = m_length * lengthScaleFactor;
    const float normalForce = gravitationConstant * m_mass * forceScaleFactor;
    const float maxFrictionForcePerWheel = normalForce * m_frictionCoefficient / m_numWheels;

    const float frontRightX = (scaledBodyWidth / 2.0f) + (scaledWheelRadius / 2.0f);
    const float frontRightY = scaledBodyLength / 4.0f;
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

    // TODO: Create new fucntion and pass params as struct?

    // TODO: When to use AUTO?
    // TODO: When to use new?
    WheelMotor* backLeftWheel = new WheelMotor(world, WheelMotor::Type::Left, scaledWheelWidth,
                                               scaledWheelRadius, scaledWheelWeight);
    jointDef.bodyB = backLeftWheel->getBody();
    jointDef.localAnchorA.Set(backLeftX, backLeftY);
    world->CreateJoint(&jointDef);
    m_wheels.push_back(backLeftWheel);

    WheelMotor* backRightWheel = new WheelMotor(world, WheelMotor::Type::Right, scaledWheelWidth,
                                                scaledWheelRadius, scaledWheelWeight);
    jointDef.bodyB = backRightWheel->getBody();
    jointDef.localAnchorA.Set(backRightX, backRightY);
    world->CreateJoint(&jointDef);
    m_wheels.push_back(backRightWheel);

    WheelMotor* frontLeftWheel = new WheelMotor(world, WheelMotor::Type::Left, scaledWheelWidth,
                                                scaledWheelRadius, scaledWheelWeight);
    jointDef.bodyB = frontLeftWheel->getBody();
    jointDef.localAnchorA.Set(frontLeftX, frontLeftY);
    m_flJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
    m_wheels.push_back(frontLeftWheel);

    WheelMotor* frontRightWheel = new WheelMotor(world, WheelMotor::Type::Right, scaledWheelWidth,
                                                 scaledWheelRadius, scaledWheelWeight);
    jointDef.bodyB = frontRightWheel->getBody();
    jointDef.localAnchorA.Set(frontRightX, frontRightY);
    m_frJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
    m_wheels.push_back(frontRightWheel);

    for (auto &wheel : m_wheels) {
        addFriction(world, wheel->getBody(), maxFrictionForcePerWheel);
    }
}

void Sumobot4Wheel::updateSensors()
{
    // TODO: Make an array to hold all sensors like the wheels...
    m_rangeSensorLeft->update();
    m_rangeSensorFrontLeft->update();
    m_rangeSensorFront->update();
    m_rangeSensorFrontRight->update();
    m_rangeSensorRight->update();
    m_rangeSensorLeft->getDistance();
    m_rangeSensorFrontLeft->getDistance();
    m_rangeSensorFront->getDistance();
    m_rangeSensorFrontRight->getDistance();
    m_rangeSensorRight->getDistance();
    m_lineDetectorFrontLeft->detected();
    m_lineDetectorFrontRight->detected();
    m_lineDetectorBackLeft->detected();
    m_lineDetectorBackRight->detected();
}

