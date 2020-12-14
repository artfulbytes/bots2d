#include "WheelMotor.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include "Body2D.h"
#include "SpriteAnimation.h"
#include <glm/glm.hpp>
#include <iostream>

WheelMotor::WheelMotor(Scene *scene, const PhysicsWorld &world, const Specification &unscaledSpec,
                                     WheelMotor::Orientation orientation, const glm::vec2 &unscaledStartPos) :
    SceneObject(scene),
    m_scaledSpec(scaleSpec(unscaledSpec))
{
    assert(world.getGravityType() == PhysicsWorld::Gravity::TopView);
    assert(unscaledSpec.maxVoltage > 0);
    m_transformComponent = std::make_unique<QuadTransform>(unscaledStartPos, glm::vec2{unscaledSpec.width, unscaledSpec.diameter});
    auto transform = static_cast<QuadTransform *>(m_transformComponent.get());

    if (unscaledSpec.textureType != WheelMotor::TextureType::None) {
        m_animation = std::make_unique<SpriteAnimation>(1, 5, 5, 3, SpriteAnimation::Direction::Backward);
        m_animation->setFramesBetweenUpdates(1);
        m_renderableComponent = std::make_unique<QuadComponent>(transform, getTexturePath(orientation, unscaledSpec.textureType), m_animation.get());
    } else {
        m_renderableComponent = std::make_unique<QuadComponent>(transform, glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
    }

    m_physicsComponent = std::make_unique<Body2D>(world, transform, Body2D::BodySpec{ true, true, unscaledSpec.mass });
    m_body2D = static_cast<Body2D *>(m_physicsComponent.get());
}

WheelMotor::~WheelMotor()
{
}

std::string WheelMotor::getTexturePath(WheelMotor::Orientation orientation, WheelMotor::TextureType textureType)
{
    switch(textureType) {
    case WheelMotor::TextureType::Orange:
        switch(orientation) {
        case WheelMotor::Orientation::Left: return "../resources/textures/wheel_sprite_left_orange.png";
        case WheelMotor::Orientation::Right: return "../resources/textures/wheel_sprite_right_orange.png";
        }
    case WheelMotor::TextureType::Green:
        switch(orientation) {
        case WheelMotor::Orientation::Left: return "../resources/textures/wheel_sprite_left_green.png";
        case WheelMotor::Orientation::Right: return "../resources/textures/wheel_sprite_right_green.png";
        }
    case TextureType::None:
        assert(0);
    }
    return "";
}

void WheelMotor::setAnimation()
{
    assert(m_animation);
    const float currentForwardSpeed = m_body2D->getForwardSpeed();
    if (currentForwardSpeed > 0.05f) {
        m_animation->setFramesBetweenUpdates(0);
        m_animation->setDirection(SpriteAnimation::Direction::Backward);
    } else if (currentForwardSpeed < -0.05f) {
        m_animation->setFramesBetweenUpdates(0);
        m_animation->setDirection(SpriteAnimation::Direction::Forward);
    } else {
        m_animation->stop();
    }
}

WheelMotor::Specification WheelMotor::scaleSpec(const Specification &unscaledSpec)
{
    const Specification scaledSpec = {
        .voltageInConstant = unscaledSpec.voltageInConstant,
        .angularSpeedConstant = unscaledSpec.angularSpeedConstant,
        .maxVoltage = unscaledSpec.maxVoltage,
        .diameter = PhysicsWorld::scaleLength(unscaledSpec.diameter),
        .width = PhysicsWorld::scaleLength(unscaledSpec.width),
        .mass = PhysicsWorld::scaleMass(unscaledSpec.mass),
        .maxDriveForce = PhysicsWorld::scaleForce(unscaledSpec.maxDriveForce),
        .maxLateralCancelingImpulse = unscaledSpec.maxLateralCancelingImpulse
    };
    return scaledSpec;
}

void WheelMotor::setVoltageIn(float voltage)
{
    assert(abs(voltage) <= m_scaledSpec.maxVoltage);
    m_voltageIn = voltage;
    updateForce();
}

float *WheelMotor::getVoltageLine()
{
    return &m_voltageIn;
}

void WheelMotor::updateForce()
{
    const glm::vec2 currentForwardNormal = m_body2D->getForwardNormal();
    const float currentForwardSpeed = m_body2D->getForwardSpeed();
    const float angularSpeed = currentForwardSpeed / (3.14f * m_scaledSpec.diameter);

    const float torqueApplied = m_scaledSpec.voltageInConstant * m_voltageIn - m_scaledSpec.angularSpeedConstant * angularSpeed;

    const float rollingFriction = 0;

    // Convert torque to force (t = r * F => F = t / r)
    const float forceToApply = (torqueApplied / (m_scaledSpec.diameter / 2)) - rollingFriction;

    m_body2D->setForce(currentForwardNormal, forceToApply);

    // Cancel lateral velocity to prevent wheel from moving sideways
    // (mimmicks sideway friction)
    m_body2D->getLateralVelocity();
    m_body2D->getMass();

    glm::vec2 lateralCancelingImpulse = m_body2D->getLateralVelocity();
    lateralCancelingImpulse *= -m_body2D->getMass();

    // Allow some skidding
    if (lateralCancelingImpulse.length() > m_scaledSpec.maxLateralCancelingImpulse) {
        //std::cout << "Skidding" << std::endl;
        lateralCancelingImpulse.x *= m_scaledSpec.maxLateralCancelingImpulse / lateralCancelingImpulse.length();
        lateralCancelingImpulse.y *= m_scaledSpec.maxLateralCancelingImpulse / lateralCancelingImpulse.length();
    }
    m_body2D->setLinearImpulse(lateralCancelingImpulse);
}

void WheelMotor::onFixedUpdate(double stepTime)
{
    if (m_animation != nullptr) {
        setAnimation();
    }
    updateForce();
}
