#include "TopViewSumobot4Wheel.h"
#include "TopViewWheelMotor.h"
#include "Body2D.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include <glm/glm.hpp>

TopViewSumobot4Wheel::TopViewSumobot4Wheel(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2 &unscaledStartPos) :
    AppObject(appScene),
    m_scaledSpec(scaleSpec(unscaledSpec))
{
    createBody(appScene, world, unscaledSpec, unscaledStartPos);
    createWheelMotors(appScene, world, unscaledSpec, unscaledStartPos);
}

void TopViewSumobot4Wheel::createBody(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2 &unscaledBodyStartPos)
{
    QuadTransform *transformBody = new QuadTransform();
    transformBody->position.x = unscaledBodyStartPos.x;
    transformBody->position.y = unscaledBodyStartPos.y;
    transformBody->position.z = 0.0f;
    transformBody->size.x = unscaledSpec.width * widthBodyFactor;
    transformBody->size.y = unscaledSpec.length;

    glm::vec4 color(1.0f, 1.0f, 1.0f, 0.2f);
    QuadComponent *renderable = new QuadComponent(color);
    m_body2D = new Body2D(world, *transformBody, true, unscaledSpec.mass * massBodyFactor);
    appScene.getScene()->createObject(transformBody, renderable, m_body2D, nullptr);
}

void TopViewSumobot4Wheel::createWheelMotors(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2 &unscaledBodyStartPos)
{
    assert(m_body2D);

    const float unscaledBodyWidth = (unscaledSpec.width * widthBodyFactor);
    const TopViewWheelMotor::Specification unscaledWheelSpec = {
        .voltageInConstant = 314.0f,
        .angularSpeedConstant = 89.0f,
        .maxVoltage = 6.0f,
        .diameter = unscaledSpec.length / 4.0f,
        .width = (unscaledSpec.width * widthWheelsFactor) / 2.0f,
        .mass = 0.05f
    };

    const Vec2 frontRightStartPos = { unscaledBodyStartPos.x + (unscaledBodyWidth / 2.0f) + (unscaledWheelSpec.width / 2.0f),
                                      unscaledBodyStartPos.y + unscaledSpec.length / 4.0f };
    const Vec2 frontLeftStartPos = { -frontRightStartPos.x, frontRightStartPos.y };
    const Vec2 backRightStartPos = { frontRightStartPos.x, -frontRightStartPos.y };
    const Vec2 backLeftStartPos = { frontLeftStartPos.x, -frontLeftStartPos.y };

    m_frontRightWheelMotor = new TopViewWheelMotor(appScene, world, unscaledWheelSpec, frontRightStartPos);
    m_body2D->attachBodyWithRevoluteJoint(frontRightStartPos, *m_frontRightWheelMotor->getBody());

    m_frontLeftWheelMotor = new TopViewWheelMotor(appScene, world, unscaledWheelSpec, frontLeftStartPos);
    m_body2D->attachBodyWithRevoluteJoint(frontLeftStartPos, *m_frontLeftWheelMotor->getBody());

    m_backRightWheelMotor = new TopViewWheelMotor(appScene, world, unscaledWheelSpec, backRightStartPos);
    m_body2D->attachBodyWithRevoluteJoint(backRightStartPos, *m_backRightWheelMotor->getBody());

    m_backLeftWheelMotor = new TopViewWheelMotor(appScene, world, unscaledWheelSpec, backLeftStartPos);
    m_body2D->attachBodyWithRevoluteJoint(backLeftStartPos, *m_backLeftWheelMotor->getBody());
}

float *TopViewSumobot4Wheel::getFrontLeftMotorVoltageLine()
{
    assert(m_frontLeftWheelMotor);
    return m_frontLeftWheelMotor->getVoltageLine();
}

float *TopViewSumobot4Wheel::getFrontRightMotorVoltageLine()
{
    assert(m_frontRightWheelMotor);
    return m_frontRightWheelMotor->getVoltageLine();
}

float *TopViewSumobot4Wheel::getBackLeftMotorVoltageLine()
{
    assert(m_backLeftWheelMotor);
    return m_backLeftWheelMotor->getVoltageLine();
}

float *TopViewSumobot4Wheel::getBackRightWheelVoltageLine()
{
    assert(m_backRightWheelMotor);
    return m_backRightWheelMotor->getVoltageLine();
}

TopViewSumobot4Wheel::Specification TopViewSumobot4Wheel::scaleSpec(const Specification &unscaledSpec)
{
    const Specification scaledSpec = {
        .length = PhysicsWorld::scaleLength(unscaledSpec.length),
        .width = PhysicsWorld::scaleLength(unscaledSpec.width),
        .mass = PhysicsWorld::scaleMass(unscaledSpec.mass)
    };
    return scaledSpec;
}

void TopViewSumobot4Wheel::onFixedUpdate(double stepTime)
{
    m_frontRightWheelMotor->onFixedUpdate(stepTime);
}

TopViewSumobot4Wheel::~TopViewSumobot4Wheel()
{
}

