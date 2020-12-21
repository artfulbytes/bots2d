#include "robots/Sumobot4Wheel.h"
#include "PhysicsWorld.h"
#include "components/Transforms.h"
#include "components/Body2D.h"
#include "components/QuadComponent.h"
#include "components/CircleComponent.h"
#include "components/LineComponent.h"
#include "actuators/WheelMotor.h"
#include "sensors/RangeSensorObject.h"
#include "sensors/LineDetectorObject.h"

namespace {
WheelMotor::TextureType getWheelTextureType(Sumobot4Wheel::TextureType textureType)
{
    switch(textureType) {
    case Sumobot4Wheel::TextureType::Plated: return WheelMotor::TextureType::Green;
    case Sumobot4Wheel::TextureType::Circuited: return WheelMotor::TextureType::Orange;
    case Sumobot4Wheel::TextureType::None: return WheelMotor::TextureType::None;
    }
    return WheelMotor::TextureType::None;
}
}

Sumobot4Wheel::Sumobot4Wheel(Scene *scene, const PhysicsWorld &world, const Specification &unscaledSpec, const glm::vec2 &unscaledStartPos) :
    SceneObject(scene),
    m_scaledSpec(scaleSpec(unscaledSpec))
{
    assert(world.getGravityType() == PhysicsWorld::Gravity::TopView);
    createBody(world, unscaledSpec, unscaledStartPos);
    createWheelMotors(world, unscaledSpec, unscaledStartPos);
    createSensors(world);

    assert(m_body2D);
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

Sumobot4Wheel::~Sumobot4Wheel()
{
}

void Sumobot4Wheel::createSensors(const PhysicsWorld &world)
{
    const RangeSensorObject::Specification leftSpec =
    {
        { -0.04f, 0.0f },
        -1.57f,
        0.0f,
        0.8f
    };
    m_leftRangeSensor = std::make_unique<RangeSensorObject>(m_scene, world, *m_body2D, true, leftSpec);
    const RangeSensorObject::Specification frontLeftSpec =
    {
        { -0.03f, 0.05f },
        -0.3f,
        0.0f,
        0.8f
    };
    m_frontLeftRangeSensor = std::make_unique<RangeSensorObject>(m_scene, world, *m_body2D, true, frontLeftSpec);
    const RangeSensorObject::Specification frontSpec =
    {
        { 0.0f, 0.05f },
        0.0f,
        0.0f,
        0.8f
    };
    m_frontRangeSensor = std::make_unique<RangeSensorObject>(m_scene, world, *m_body2D, true, frontSpec);
    const RangeSensorObject::Specification frontRightSpec =
    {
        { 0.03f, 0.05f },
        0.3f,
        0.0f,
        0.8f

    };
    m_frontRightRangeSensor = std::make_unique<RangeSensorObject>(m_scene, world, *m_body2D, true, frontRightSpec);
    const RangeSensorObject::Specification rightSpec =
    {
        { 0.04f, 0.0f },
        1.57f,
        0.0f,
        0.8f
    };
    m_rightRangeSensor = std::make_unique<RangeSensorObject>(m_scene, world, *m_body2D, true, rightSpec);

    m_frontLeftLineDetector = std::make_unique<LineDetectorObject>(m_scene, world, *m_body2D, true, glm::vec2{-0.035f, 0.045f});
    m_frontRightLineDetector = std::make_unique<LineDetectorObject>(m_scene, world, *m_body2D, true, glm::vec2{0.035f, 0.045f});
    m_backLeftLineDetector = std::make_unique<LineDetectorObject>(m_scene, world, *m_body2D, true, glm::vec2{-0.035f, -0.045f});
    m_backRightLineDetector = std::make_unique<LineDetectorObject>(m_scene, world, *m_body2D, true, glm::vec2{0.035f, -0.045f});
}

void Sumobot4Wheel::createBody(const PhysicsWorld &world, const Specification &unscaledSpec, const glm::vec2 &unscaledBodyStartPos)
{
    m_transformComponent = std::make_unique<QuadTransform>(glm::vec2{ unscaledBodyStartPos.x, unscaledBodyStartPos.y },
                                                           glm::vec2{ unscaledSpec.width * widthBodyFactor, unscaledSpec.length });
    const auto transform = static_cast<QuadTransform *>(m_transformComponent.get());

    switch(unscaledSpec.textureType) {
    case Sumobot4Wheel::TextureType::Plated:
        m_renderableComponent = std::make_unique<QuadComponent>(transform, "../resources/textures/sumobot_body_plated.png");
        break;
    case Sumobot4Wheel::TextureType::Circuited:
        m_renderableComponent = std::make_unique<QuadComponent>(transform, "../resources/textures/sumobot_body_circuited.png");
        break;
    case Sumobot4Wheel::TextureType::None:
        glm::vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
        m_renderableComponent = std::make_unique<QuadComponent>(transform, color);
        break;
    }
    m_physicsComponent = std::make_unique<Body2D>(world, transform, Body2D::BodySpec{ true, true, unscaledSpec.mass * massBodyFactor });
    m_body2D = static_cast<Body2D *>(m_physicsComponent.get());
}

void Sumobot4Wheel::createWheelMotors(const PhysicsWorld &world, const Specification &unscaledSpec, const glm::vec2 &unscaledBodyStartPos)
{
    assert(m_body2D);

    const float unscaledBodyWidth = (unscaledSpec.width * widthBodyFactor);
    const WheelMotor::Specification unscaledWheelSpec = {
        314.0f,
        89.0f,
        6.0f,
        unscaledSpec.length / 4.0f,
        (unscaledSpec.width * widthWheelsFactor) / 2.0f,
        0.05f,
        40.5f,
        getWheelTextureType(unscaledSpec.textureType)
    };

    float frontYPos = 0.0f;
    switch(unscaledSpec.textureType) {
    case Sumobot4Wheel::TextureType::Plated:
        frontYPos = unscaledSpec.length / 7.0f;
        break;
    case Sumobot4Wheel::TextureType::Circuited:
        // Fallthrough
    case Sumobot4Wheel::TextureType::None:
        frontYPos = unscaledSpec.length / 5.0f;
        break;
    }
    const glm::vec2 frontRightStartPos = { (unscaledBodyWidth / 2.0f) + (unscaledWheelSpec.width / 2.0f), frontYPos };
    const glm::vec2 frontLeftStartPos = { -frontRightStartPos.x, frontRightStartPos.y };
    const glm::vec2 backRightStartPos = { frontRightStartPos.x, -(unscaledSpec.length / 3.5f) };
    const glm::vec2 backLeftStartPos = { frontLeftStartPos.x, backRightStartPos.y };

    m_frontRightWheelMotor = std::make_unique<WheelMotor>(m_scene, world, unscaledWheelSpec, WheelMotor::Orientation::Right, frontRightStartPos);
    m_body2D->attachBodyWithRevoluteJoint(frontRightStartPos, m_frontRightWheelMotor->getBody());

    m_frontLeftWheelMotor = std::make_unique<WheelMotor>(m_scene, world, unscaledWheelSpec, WheelMotor::Orientation::Left, frontLeftStartPos);
    m_body2D->attachBodyWithRevoluteJoint(frontLeftStartPos, m_frontLeftWheelMotor->getBody());

    m_backRightWheelMotor = std::make_unique<WheelMotor>(m_scene, world, unscaledWheelSpec, WheelMotor::Orientation::Right, backRightStartPos);
    m_body2D->attachBodyWithRevoluteJoint(backRightStartPos, m_backRightWheelMotor->getBody());

    m_backLeftWheelMotor = std::make_unique<WheelMotor>(m_scene, world, unscaledWheelSpec, WheelMotor::Orientation::Left, backLeftStartPos);
    m_body2D->attachBodyWithRevoluteJoint(backLeftStartPos, m_backLeftWheelMotor->getBody());
}

float *Sumobot4Wheel::getVoltageLine(Sumobot4Wheel::VoltageLine line) const
{
    switch (line) {
    case Sumobot4Wheel::VoltageLine::FrontLeftMotor: return m_frontLeftWheelMotor->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::BackLeftMotor: return m_backLeftWheelMotor->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::FrontRightMotor: return m_frontRightWheelMotor->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::BackRightMotor: return m_backRightWheelMotor->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::LeftRangeSensor: return m_leftRangeSensor->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::FrontLeftRangeSensor: return m_frontLeftRangeSensor->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::FrontRangeSensor: return m_frontRangeSensor->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::FrontRightRangeSensor: return m_frontRightRangeSensor->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::RightRangeSensor: return m_rightRangeSensor->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::FrontLeftLineDetector: return m_frontLeftLineDetector->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::BackLeftLineDetector: return m_backLeftLineDetector->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::FrontRightLineDetector: return m_frontRightLineDetector->getVoltageLine();
    case Sumobot4Wheel::VoltageLine::BackRightLineDetector: return m_backRightLineDetector->getVoltageLine();
    }
    assert(0);
    return nullptr;
}

Sumobot4Wheel::Specification Sumobot4Wheel::scaleSpec(const Specification &unscaledSpec)
{
    const Specification scaledSpec = {
        PhysicsWorld::scaleLength(unscaledSpec.length),
        PhysicsWorld::scaleLength(unscaledSpec.width),
        PhysicsWorld::scaleMass(unscaledSpec.mass)
    };
    return scaledSpec;
}

void Sumobot4Wheel::onFixedUpdate(double stepTime)
{
}
