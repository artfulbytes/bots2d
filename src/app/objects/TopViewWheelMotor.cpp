#include "TopViewWheelMotor.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include "SpriteAnimation.h"
#include <glm/glm.hpp>
#include <iostream>

TopViewWheelMotor::TopViewWheelMotor(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec,
                                     TopViewWheelMotor::Orientation orientation, const Vec2<float> &unscaledStartPos) :
    AppObject(appScene),
    m_scaledSpec(scaleSpec(unscaledSpec))
{
    assert(world.getGravityType() == PhysicsWorld::Gravity::TopView);
    assert(unscaledSpec.maxVoltage > 0);
    QuadTransform *transformBody = new QuadTransform();
    transformBody->position.x = unscaledStartPos.x;
    transformBody->position.y = unscaledStartPos.y;
    transformBody->position.z = 0.0f;
    transformBody->size.x = unscaledSpec.width;
    transformBody->size.y = unscaledSpec.diameter;

    QuadComponent *renderable = nullptr;
    if (unscaledSpec.textureType != TopViewWheelMotor::TextureType::None) {
        m_animation = new SpriteAnimation(Vec2<unsigned int>{1, 5}, 5, 3, SpriteAnimation::Direction::Backward);
        m_animation->setFramesBetweenUpdates(1);
        renderable = new QuadComponent(getTexturePath(orientation, unscaledSpec.textureType), *m_animation);
    } else {
        renderable = new QuadComponent(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
    }

    m_body2D = new Body2D(world, *transformBody, true, true, unscaledSpec.mass);
    appScene.getScene()->createObject(transformBody, renderable, m_body2D, nullptr);
}

TopViewWheelMotor::~TopViewWheelMotor()
{
}

std::string TopViewWheelMotor::getTexturePath(TopViewWheelMotor::Orientation orientation, TopViewWheelMotor::TextureType textureType)
{
    switch(textureType) {
    case TopViewWheelMotor::TextureType::Orange:
        switch(orientation) {
        case TopViewWheelMotor::Orientation::Left: return "../resources/textures/wheel_sprite_left_orange.png";
        case TopViewWheelMotor::Orientation::Right: return "../resources/textures/wheel_sprite_right_orange.png";
        }
    case TopViewWheelMotor::TextureType::Green:
        switch(orientation) {
        case TopViewWheelMotor::Orientation::Left: return "../resources/textures/wheel_sprite_left_green.png";
        case TopViewWheelMotor::Orientation::Right: return "../resources/textures/wheel_sprite_right_green.png";
        }
    case TextureType::None:
        assert(0);
    }
    return "";
}

void TopViewWheelMotor::setAnimation()
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

TopViewWheelMotor::Specification TopViewWheelMotor::scaleSpec(const Specification &unscaledSpec)
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

void TopViewWheelMotor::setVoltageIn(float voltage)
{
    assert(abs(voltage) <= m_scaledSpec.maxVoltage);
    m_voltageIn = voltage;
    updateForce();
}

float *TopViewWheelMotor::getVoltageLine()
{
    return &m_voltageIn;
}

void TopViewWheelMotor::updateForce()
{
    const Vec2<float> currentForwardNormal = m_body2D->getForwardNormal();
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

    Vec2<float> lateralCancelingImpulse = m_body2D->getLateralVelocity();
    lateralCancelingImpulse.x *= -m_body2D->getMass();
    lateralCancelingImpulse.y *= -m_body2D->getMass();

    // Allow some skidding
    if (lateralCancelingImpulse.length() > m_scaledSpec.maxLateralCancelingImpulse) {
        //std::cout << "Skidding" << std::endl;
        lateralCancelingImpulse.x *= m_scaledSpec.maxLateralCancelingImpulse / lateralCancelingImpulse.length();
        lateralCancelingImpulse.y *= m_scaledSpec.maxLateralCancelingImpulse / lateralCancelingImpulse.length();
    }
    m_body2D->setLinearImpulse(lateralCancelingImpulse);
}

void TopViewWheelMotor::onFixedUpdate(double stepTime)
{
    if (m_animation != nullptr) {
        setAnimation();
    }
    updateForce();
}
