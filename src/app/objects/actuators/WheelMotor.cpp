#include "actuators/WheelMotor.h"
#include "PhysicsWorld.h"
#include "components/Transforms.h"
#include "components/QuadComponent.h"
#include "components/Body2D.h"
#include "SpriteAnimation.h"
#include "ResourcesHelper.h"

#include <glm/glm.hpp>

WheelMotor::WheelMotor(Scene *scene, const Specification &spec, WheelMotor::Orientation orientation,
                       const glm::vec2 &startPosition, float startRotation) :
    SceneObject(scene),
    m_spec(spec)
{
    assert(m_physicsWorld->getGravityType() == PhysicsWorld::Gravity::TopView);
    assert(spec.maxVoltage > 0);
    m_transformComponent = std::make_unique<QuadTransform>(startPosition, glm::vec2{spec.width, spec.diameter}, startRotation);
    auto transform = static_cast<QuadTransform *>(m_transformComponent.get());

    if (spec.textureType != WheelMotor::TextureType::None) {
        m_animation = std::make_unique<SpriteAnimation>(1, 5, 5, 3, SpriteAnimation::Direction::Backward);
        m_animation->setFramesBetweenUpdates(1);
        m_renderableComponent = std::make_unique<QuadComponent>(transform, getTextureName(orientation, spec.textureType), m_animation.get());
    } else {
        m_renderableComponent = std::make_unique<QuadComponent>(transform, glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
    }

    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, Body2D::Specification{ true, true, spec.mass });
    m_body2D = static_cast<Body2D *>(m_physicsComponent.get());
}

WheelMotor::~WheelMotor()
{
}

std::string WheelMotor::getTextureName(WheelMotor::Orientation orientation, WheelMotor::TextureType textureType) const
{
    switch(textureType) {
    case WheelMotor::TextureType::Orange:
        switch(orientation) {
        case WheelMotor::Orientation::Left: return "wheel_sprite_left_orange.png";
        case WheelMotor::Orientation::Right: return "wheel_sprite_right_orange.png";
        }
    case WheelMotor::TextureType::Green:
        switch(orientation) {
        case WheelMotor::Orientation::Left: return "wheel_sprite_left_green.png";
        case WheelMotor::Orientation::Right: return "wheel_sprite_right_green.png";
        }
    case WheelMotor::TextureType::Red:
        switch(orientation) {
        case WheelMotor::Orientation::Left: return "wheel_sprite_left_red.png";
        case WheelMotor::Orientation::Right: return "wheel_sprite_right_red.png";
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
    if (currentForwardSpeed > 0.05f || m_voltageIn > 0.0f) {
        m_animation->setFramesBetweenUpdates(0);
        m_animation->setDirection(SpriteAnimation::Direction::Backward);
    } else if (currentForwardSpeed < -0.05f || m_voltageIn < 0.0f) {
        m_animation->setFramesBetweenUpdates(0);
        m_animation->setDirection(SpriteAnimation::Direction::Forward);
    } else {
        m_animation->stop();
    }
}

void WheelMotor::setVoltageIn(float voltage)
{
    assert(abs(voltage) <= m_spec.maxVoltage);
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
    const float diameter = m_spec.diameter;
    const float angularSpeed = currentForwardSpeed / (3.14f * diameter);
    const float torqueApplied = m_spec.voltageInConstant * m_voltageIn - m_spec.angularSpeedConstant * angularSpeed;
    const float rollingFriction = 0;
    // Convert torque to force (t = r * F => F = t / r)
    const float forceToApply = (torqueApplied / (diameter / 2)) - rollingFriction;
    m_body2D->setForce(currentForwardNormal, forceToApply);
}

void WheelMotor::onFixedUpdate(double stepTime)
{
    if (m_animation != nullptr) {
        setAnimation();
    }
    updateForce();
}
