#include "actuators/WheelMotor.h"
#include "PhysicsWorld.h"
#include "components/Transforms.h"
#include "components/RectComponent.h"
#include "components/Body2D.h"
#include "SpriteAnimation.h"
#include "AssetsHelper.h"

#include <glm/glm.hpp>
#include <iostream>

WheelMotor::WheelMotor(Scene *scene, const Specification &spec, WheelMotor::Orientation orientation,
                       const glm::vec2 &startPosition, float startRotation) :
    SceneObject(scene),
    m_spec(spec)
{
    assert(m_physicsWorld->getGravityType() == PhysicsWorld::Gravity::TopView);
    assert(spec.maxVoltage > 0);
    m_transformComponent = std::make_unique<RectTransform>(startPosition, glm::vec2{spec.width, spec.diameter}, startRotation);
    auto transform = static_cast<RectTransform *>(m_transformComponent.get());

    if (spec.textureType != WheelMotor::TextureType::None) {
        m_animation = std::make_unique<SpriteAnimation>(1, 5, 5, 3, SpriteAnimation::Direction::Backward);
        m_animation->setFramesBetweenUpdates(1);
        m_renderableComponent = std::make_unique<RectComponent>(transform, getTextureName(orientation, spec.textureType), m_animation.get());
    } else {
        m_renderableComponent = std::make_unique<RectComponent>(transform, glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
    }

    Body2D::Specification bodySpec(true, true, spec.wheelMass + spec.loadedMass, spec.frictionCoefficient);
    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, bodySpec);
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
        break;
    case WheelMotor::TextureType::Green:
        switch(orientation) {
        case WheelMotor::Orientation::Left: return "wheel_sprite_left_green.png";
        case WheelMotor::Orientation::Right: return "wheel_sprite_right_green.png";
        }
        break;
    case WheelMotor::TextureType::Red:
        switch(orientation) {
        case WheelMotor::Orientation::Left: return "wheel_sprite_left_red.png";
        case WheelMotor::Orientation::Right: return "wheel_sprite_right_red.png";
        }
        break;
    case TextureType::None:
        assert(0);
    }
    return "";
}

void WheelMotor::setAnimation()
{
    assert(m_animation);
    const float currentForwardSpeed = m_body2D->getForwardSpeed();
    if (m_enabled && (currentForwardSpeed > 0.02f || m_voltageIn > 0.0f)) {
        m_animation->setFramesBetweenUpdates(0);
        m_animation->setDirection(SpriteAnimation::Direction::Backward);
    } else if (m_enabled && (currentForwardSpeed < -0.02f || m_voltageIn < 0.0f)) {
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
}

float *WheelMotor::getVoltageLine()
{
    return &m_voltageIn;
}

void WheelMotor::updateForce()
{
    const glm::vec2 currentForwardNormal = m_body2D->getForwardNormal();
    if (!m_enabled) {
        m_body2D->setForce(currentForwardNormal, 0.0f);
        return;
    }
    const float currentForwardSpeed = m_body2D->getForwardSpeed();
    const float diameter = m_spec.diameter;
    const float angularSpeed = currentForwardSpeed / (3.14f * diameter);
    const float torqueApplied = m_spec.voltageInConstant * m_voltageIn - m_spec.angularSpeedConstant * angularSpeed;
    /* Convert torque to force (t = r * F => F = t / r) */
    const float forceToApply = (torqueApplied / (diameter / 2));
    m_body2D->setForce(currentForwardNormal, forceToApply);

    /* Apply sideway friction to mimic real wheel */
    glm::vec2 lateralCancelingImpulse = -m_body2D->getLateralVelocity() * m_spec.sidewayFrictionConstant;
    m_body2D->setLinearImpulse(lateralCancelingImpulse);
}

void WheelMotor::onFixedUpdate()
{
    if (m_animation != nullptr) {
        setAnimation();
    }
    updateForce();
}

void WheelMotor::setSidewayFrictionConstant(float sidewayFrictionConstant)
{
    m_spec.sidewayFrictionConstant = sidewayFrictionConstant;
}

void WheelMotor::setFrictionCoefficient(float frictionCoefficient)
{
    m_spec.frictionCoefficient = frictionCoefficient;
    m_body2D->setFrictionCoefficient(frictionCoefficient);
}

float WheelMotor::getFrictionCoefficient() const
{
    return m_spec.frictionCoefficient;
}

float WheelMotor::getSidewayFrictionConstant() const
{
    return m_spec.sidewayFrictionConstant;
}

void WheelMotor::setLoadedMass(float loadedMass) {
    assert(loadedMass >= 0);

    m_spec.loadedMass = loadedMass;
    m_body2D->setMass(m_spec.wheelMass + m_spec.loadedMass);
}

void WheelMotor::setMass(float mass) {
    assert(mass >= 0);

    m_spec.wheelMass = mass;
    m_body2D->setMass(m_spec.wheelMass + m_spec.loadedMass);
}

float WheelMotor::getMass() const
{
    return m_body2D->getMass();
}

void WheelMotor::setMaxVoltage(float maxVoltage)
{
    assert(maxVoltage >= 0);
    m_spec.maxVoltage = maxVoltage;
}

void WheelMotor::setAngularSpeedConstant(float angularSpeedConstant)
{
    assert(angularSpeedConstant >= 0);
    m_spec.angularSpeedConstant = angularSpeedConstant;
}

void WheelMotor::setVoltageInConstant(float voltageInConstant)
{
    assert(voltageInConstant >= 0);
    m_spec.voltageInConstant = voltageInConstant;
}

float WheelMotor::getVoltageInConstant() const
{
    return m_spec.voltageInConstant;
}

float WheelMotor::getMaxVoltage() const
{
    return m_spec.maxVoltage;
}

float WheelMotor::getAngularSpeedConstant() const
{
    return m_spec.angularSpeedConstant;
}

void WheelMotor::enable()
{
    m_enabled = true;
}

void WheelMotor::disable()
{
    m_enabled = false;
}
