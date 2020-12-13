#include "TopViewSumobot4Wheel.h"
#include "TopViewWheelMotor.h"
#include "Body2D.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include "CircleComponent.h"
#include "LineComponent.h"
#include "RangeSensorObject.h"
#include "LineDetectorObject.h"
#include <glm/glm.hpp>

namespace {
TopViewWheelMotor::TextureType getWheelTextureType(TopViewSumobot4Wheel::TextureType textureType)
{
    switch(textureType) {
    case TopViewSumobot4Wheel::TextureType::Plated: return TopViewWheelMotor::TextureType::Green;
    case TopViewSumobot4Wheel::TextureType::Circuited: return TopViewWheelMotor::TextureType::Orange;
    case TopViewSumobot4Wheel::TextureType::None: return TopViewWheelMotor::TextureType::None;
    }
    return TopViewWheelMotor::TextureType::None;
}
}

TopViewSumobot4Wheel::TopViewSumobot4Wheel(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2<float> &unscaledStartPos) :
    AppObject(appScene),
    m_scaledSpec(scaleSpec(unscaledSpec))
{
    assert(world.getGravityType() == PhysicsWorld::Gravity::TopView);
    createBody(appScene, world, unscaledSpec, unscaledStartPos);
    createWheelMotors(appScene, world, unscaledSpec, unscaledStartPos);
    createSensors(appScene, world);

    assert(m_frontRightWheelMotor);
    assert(m_frontLeftWheelMotor);
    assert(m_backRightWheelMotor);
    assert(m_backLeftWheelMotor);
    assert(m_leftRangeSensor);
    assert(m_frontLeftRangeSensor);
    assert(m_frontRangeSensor);
    assert(m_frontRightRangeSensor);
    assert(m_rightRangeSensor);
    assert(m_frontLeftLineDetector);
    assert(m_frontRightLineDetector);
    assert(m_backLeftLineDetector);
    assert(m_backRightLineDetector);
}

TopViewSumobot4Wheel::~TopViewSumobot4Wheel()
{
}

void TopViewSumobot4Wheel::createSensors(AppScene &appScene, const PhysicsWorld &world)
{
    m_leftRangeSensor = new RangeSensorObject(appScene, world, *m_body2D, true,
                                              { .relativePosition = { -0.04f, 0.0f },
                                                .relativeAngle = -1.57f,
                                                .minDistance = 0.0f,
                                                .maxDistance = 0.8f });
    m_frontLeftRangeSensor = new RangeSensorObject(appScene, world, *m_body2D, true,
                                                   { .relativePosition = { -0.03f, 0.05f },
                                                     .relativeAngle = -0.3f,
                                                     .minDistance = 0.0f,
                                                     .maxDistance = 0.8f });
    m_frontRangeSensor = new RangeSensorObject(appScene, world, *m_body2D, true,
                                               { .relativePosition = { 0.0f, 0.05f },
                                                 .relativeAngle = 0.0f,
                                                 .minDistance = 0.0f,
                                                 .maxDistance = 0.8f });
    m_frontRightRangeSensor = new RangeSensorObject(appScene, world, *m_body2D, true,
                                                    { .relativePosition = { 0.03f, 0.05f },
                                                      .relativeAngle = 0.3f,
                                                      .minDistance = 0.0f,
                                                      .maxDistance = 0.8f });
    m_rightRangeSensor = new RangeSensorObject(appScene, world, *m_body2D, true,
                                               { .relativePosition = { 0.04f, 0.0f },
                                                 .relativeAngle = 1.57f,
                                                 .minDistance = 0.0f,
                                                 .maxDistance = 0.8f });

    m_frontLeftLineDetector = new LineDetectorObject(appScene, world, *m_body2D, true, Vec2<float>{-0.035f, 0.045f});
    m_frontRightLineDetector = new LineDetectorObject(appScene, world, *m_body2D, true, Vec2<float>{0.035f, 0.045f});
    m_backLeftLineDetector = new LineDetectorObject(appScene, world, *m_body2D, true, Vec2<float>{-0.035f, -0.045f});
    m_backRightLineDetector = new LineDetectorObject(appScene, world, *m_body2D, true, Vec2<float>{0.035f, -0.045f});
}

void TopViewSumobot4Wheel::createBody(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2<float> &unscaledBodyStartPos)
{
    QuadTransform *transformBody = new QuadTransform();
    transformBody->position.x = unscaledBodyStartPos.x;
    transformBody->position.y = unscaledBodyStartPos.y;
    transformBody->position.z = 0.0f;
    transformBody->size.x = unscaledSpec.width * widthBodyFactor;
    transformBody->size.y = unscaledSpec.length;

    QuadComponent *renderable = nullptr;

    switch(unscaledSpec.textureType) {
    case TopViewSumobot4Wheel::TextureType::Plated:
        renderable = new QuadComponent("../resources/textures/sumobot_body_plated.png");
        break;
    case TopViewSumobot4Wheel::TextureType::Circuited:
        renderable = new QuadComponent("../resources/textures/sumobot_body_circuited.png");
        break;
    case TopViewSumobot4Wheel::TextureType::None:
        glm::vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
        renderable = new QuadComponent(color);
        break;
    }
    m_body2D = new Body2D(world, *transformBody, true, true, unscaledSpec.mass * massBodyFactor);
    appScene.getScene()->createObject(transformBody, renderable, m_body2D, nullptr);
}

void TopViewSumobot4Wheel::createWheelMotors(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2<float> &unscaledBodyStartPos)
{
    assert(m_body2D);

    const float unscaledBodyWidth = (unscaledSpec.width * widthBodyFactor);
    const TopViewWheelMotor::Specification unscaledWheelSpec = {
        .voltageInConstant = 314.0f,
        .angularSpeedConstant = 89.0f,
        .maxVoltage = 6.0f,
        .diameter = unscaledSpec.length / 4.0f,
        .width = (unscaledSpec.width * widthWheelsFactor) / 2.0f,
        .mass = 0.05f,
        .textureType = getWheelTextureType(unscaledSpec.textureType)
    };

    float frontYPos = 0.0f;
    switch(unscaledSpec.textureType) {
    case TopViewSumobot4Wheel::TextureType::Plated:
        frontYPos = unscaledSpec.length / 7.0f;
        break;
    case TopViewSumobot4Wheel::TextureType::Circuited:
        // Fallthrough
    case TopViewSumobot4Wheel::TextureType::None:
        frontYPos = unscaledSpec.length / 5.0f;
        break;
    }
    const Vec2<float> frontRightStartPos = { (unscaledBodyWidth / 2.0f) + (unscaledWheelSpec.width / 2.0f),
                                             frontYPos };
    const Vec2<float> frontLeftStartPos = { -frontRightStartPos.x, frontRightStartPos.y };

    const Vec2<float> backRightStartPos = { frontRightStartPos.x, -(unscaledSpec.length / 3.5f) };
    const Vec2<float> backLeftStartPos = { frontLeftStartPos.x, backRightStartPos.y };

    m_frontRightWheelMotor = new TopViewWheelMotor(appScene, world, unscaledWheelSpec, TopViewWheelMotor::Orientation::Right, frontRightStartPos);
    m_body2D->attachBodyWithRevoluteJoint(frontRightStartPos, *m_frontRightWheelMotor->getBody());

    m_frontLeftWheelMotor = new TopViewWheelMotor(appScene, world, unscaledWheelSpec, TopViewWheelMotor::Orientation::Left, frontLeftStartPos);
    m_body2D->attachBodyWithRevoluteJoint(frontLeftStartPos, *m_frontLeftWheelMotor->getBody());

    m_backRightWheelMotor = new TopViewWheelMotor(appScene, world, unscaledWheelSpec, TopViewWheelMotor::Orientation::Right, backRightStartPos);
    m_body2D->attachBodyWithRevoluteJoint(backRightStartPos, *m_backRightWheelMotor->getBody());

    m_backLeftWheelMotor = new TopViewWheelMotor(appScene, world, unscaledWheelSpec, TopViewWheelMotor::Orientation::Left, backLeftStartPos);
    m_body2D->attachBodyWithRevoluteJoint(backLeftStartPos, *m_backLeftWheelMotor->getBody());
}

float *TopViewSumobot4Wheel::getVoltageLine(TopViewSumobot4Wheel::VoltageLine line) const
{
    switch (line) {
    case TopViewSumobot4Wheel::VoltageLine::FrontLeftMotor: return m_frontLeftWheelMotor->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::BackLeftMotor: return m_backLeftWheelMotor->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::FrontRightMotor: return m_frontRightWheelMotor->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::BackRightMotor: return m_backRightWheelMotor->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::LeftRangeSensor: return m_leftRangeSensor->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::FrontLeftRangeSensor: return m_frontLeftRangeSensor->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::FrontRangeSensor: return m_frontRangeSensor->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::FrontRightRangeSensor: return m_frontRightRangeSensor->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::RightRangeSensor: return m_rightRangeSensor->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::FrontLeftLineDetector: return m_frontLeftLineDetector->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::BackLeftLineDetector: return m_backLeftLineDetector->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::FrontRightLineDetector: return m_frontRightLineDetector->getVoltageLine();
    case TopViewSumobot4Wheel::VoltageLine::BackRightLineDetector: return m_backRightLineDetector->getVoltageLine();
    }
    assert(0);
    return nullptr;
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
}
