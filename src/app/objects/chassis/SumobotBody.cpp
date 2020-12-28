#include "chassis/SumobotBody.h"
#include "PhysicsWorld.h"
#include "components/Transforms.h"
#include "components/Body2D.h"
#include "components/QuadComponent.h"
#include "components/CircleComponent.h"
#include "actuators/WheelMotor.h"
#include "sensors/RangeSensorObject.h"
#include "sensors/LineDetectorObject.h"

#include <iostream>

SumobotBody::SumobotBody(Scene *scene, const Specification &spec, const glm::vec2 &startPosition, float startRotation) :
    SceneObject(scene)
{
    assert(m_physicsWorld->getGravityType() == PhysicsWorld::Gravity::TopView);
    switch (spec.shape) {
    case SumobotBody::Shape::Rectangle:
        createRectangleBody(spec, startPosition, startRotation);
        break;
    case SumobotBody::Shape::Circle:
        createCircleBody(spec, startPosition, startRotation);
        break;
    }
}

SumobotBody::~SumobotBody()
{
}

glm::vec2 SumobotBody::getPosition() const
{
    return m_body2D->getPosition();
}

float SumobotBody::getRotation() const
{
    return m_body2D->getRotation();
}

float SumobotBody::getForwardSpeed() const
{
    return m_body2D->getForwardSpeed();
}

void SumobotBody::createRectangleBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation)
{
    m_transformComponent = std::make_unique<QuadTransform>(startPosition, glm::vec2{ spec.width, spec.length }, startRotation);
    const auto transform = static_cast<QuadTransform *>(m_transformComponent.get());

    switch(spec.textureType) {
    case SumobotBody::TextureType::Plated:
        m_renderableComponent = std::make_unique<QuadComponent>(transform, "sumobot_body_plated.png");
        break;
    case SumobotBody::TextureType::Circuited:
        m_renderableComponent = std::make_unique<QuadComponent>(transform, "sumobot_body_circuited.png");
        break;
    case SumobotBody::TextureType::RoundRed:
    case SumobotBody::TextureType::RoundBlack:
        assert(0);
        std::cout << "Can't use round texture for rectangle shape" << std::endl;
        break;
    case SumobotBody::TextureType::None:
        glm::vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
        m_renderableComponent = std::make_unique<QuadComponent>(transform, color);
        break;
    }
    Body2D::Specification bodySpec(true, true, spec.mass);
    if (spec.torqueFrictionCoefficient >= 0.0f)
    {
        bodySpec.torqueFrictionCoefficient = spec.torqueFrictionCoefficient;
    }
    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, bodySpec);
    m_body2D = static_cast<Body2D *>(m_physicsComponent.get());
}

void SumobotBody::createCircleBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation)
{
    m_transformComponent = std::make_unique<CircleTransform>(startPosition, (spec.width / 2.0f), startRotation);
    const auto transform = static_cast<CircleTransform *>(m_transformComponent.get());

    switch(spec.textureType) {
    case SumobotBody::TextureType::Plated:
        m_renderableComponent = std::make_unique<QuadComponent>(transform, "sumobot_body_plated.png");
        break;
    case SumobotBody::TextureType::Circuited:
        m_renderableComponent = std::make_unique<QuadComponent>(transform, "sumobot_body_circuited.png");
        break;
    case SumobotBody::TextureType::RoundRed:
        m_renderableComponent = std::make_unique<QuadComponent>(transform, "sumobot_body_round_red.png");
        break;
    case SumobotBody::TextureType::RoundBlack:
        m_renderableComponent = std::make_unique<QuadComponent>(transform, "sumobot_body_round_black.png");
        break;
    case SumobotBody::TextureType::None:
        glm::vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
        m_renderableComponent = std::make_unique<CircleComponent>(transform, color);
        break;
    }

    Body2D::Specification bodySpec(true, true, spec.mass);
    if (spec.torqueFrictionCoefficient >= 0.0f)
    {
        bodySpec.torqueFrictionCoefficient = spec.torqueFrictionCoefficient;
    }
    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, bodySpec);
    m_body2D = static_cast<Body2D *>(m_physicsComponent.get());
}

void SumobotBody::attachWheelMotor(const WheelMotor *wheelMotor, glm::vec2 relativePosition)
{
    m_body2D->attachBodyWithRevoluteJoint(relativePosition, wheelMotor->getBody());
}

void SumobotBody::attachSensor(const RangeSensorObject *rangeSensorObject, glm::vec2 relativePosition)
{
    m_body2D->attachBodyWithWeldJoint(relativePosition, rangeSensorObject->getBody());
}

void SumobotBody::attachSensor(const LineDetectorObject *lineDetectorObject, glm::vec2 relativePosition)
{
    m_body2D->attachBodyWithWeldJoint(relativePosition, lineDetectorObject->getBody());
}

void SumobotBody::onFixedUpdate(double stepTime)
{
    (void)stepTime;
}
