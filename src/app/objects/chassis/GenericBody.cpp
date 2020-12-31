#include "chassis/GenericBody.h"
#include "PhysicsWorld.h"
#include "components/Transforms.h"
#include "components/Body2D.h"
#include "components/RectComponent.h"
#include "components/CircleComponent.h"
#include "actuators/WheelMotor.h"
#include "sensors/RangeSensorObject.h"
#include "sensors/LineDetectorObject.h"

#include <iostream>

GenericBody::GenericBody(Scene *scene, const Specification &spec, const glm::vec2 &startPosition, float startRotation) :
    SceneObject(scene)
{
    assert(m_physicsWorld->getGravityType() == PhysicsWorld::Gravity::TopView);
    switch (spec.shape) {
    case GenericBody::Shape::Rectangle:
        createRectangleBody(spec, startPosition, startRotation);
        break;
    case GenericBody::Shape::Circle:
        createCircleBody(spec, startPosition, startRotation);
        break;
    }
}

GenericBody::~GenericBody()
{
}

glm::vec2 GenericBody::getPosition() const
{
    return m_body2D->getPosition();
}

float GenericBody::getRotation() const
{
    return m_body2D->getRotation();
}

float GenericBody::getForwardSpeed() const
{
    return m_body2D->getForwardSpeed();
}

void GenericBody::createRectangleBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation)
{
    m_transformComponent = std::make_unique<RectTransform>(startPosition, glm::vec2{ spec.width, spec.length }, startRotation);
    const auto transform = static_cast<RectTransform *>(m_transformComponent.get());

    switch(spec.textureType) {
    case GenericBody::TextureType::SumobotPlated:
        m_renderableComponent = std::make_unique<RectComponent>(transform, "sumobot_body_plated.png");
        break;
    case GenericBody::TextureType::SumobotCircuited:
        m_renderableComponent = std::make_unique<RectComponent>(transform, "sumobot_body_circuited.png");
        break;
    case GenericBody::TextureType::SumobotRoundRed:
    case GenericBody::TextureType::SumobotRoundBlack:
        assert(0);
        std::cout << "Can't use round texture for rectangle shape" << std::endl;
        break;
    case GenericBody::TextureType::LineFollowerPlated:
        m_renderableComponent = std::make_unique<RectComponent>(transform, "line_follower_plated.png");
        break;
    case GenericBody::TextureType::None:
        glm::vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
        m_renderableComponent = std::make_unique<RectComponent>(transform, color);
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

void GenericBody::createCircleBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation)
{
    m_transformComponent = std::make_unique<CircleTransform>(startPosition, (spec.width / 2.0f), startRotation);
    const auto transform = static_cast<CircleTransform *>(m_transformComponent.get());

    switch(spec.textureType) {
    case GenericBody::TextureType::SumobotPlated:
    case GenericBody::TextureType::SumobotCircuited:
    case GenericBody::TextureType::LineFollowerPlated:
        assert(0);
        std::cout << "Can't use rectangular texture for round shape" << std::endl;
        break;
    case GenericBody::TextureType::SumobotRoundRed:
        m_renderableComponent = std::make_unique<RectComponent>(transform, "sumobot_body_round_red.png");
        break;
    case GenericBody::TextureType::SumobotRoundBlack:
        m_renderableComponent = std::make_unique<RectComponent>(transform, "sumobot_body_round_black.png");
        break;
    case GenericBody::TextureType::None:
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

void GenericBody::attachWheelMotor(const WheelMotor *wheelMotor, glm::vec2 relativePosition)
{
    m_body2D->attachBodyWithRevoluteJoint(relativePosition, wheelMotor->getBody());
}

void GenericBody::attachSensor(const RangeSensorObject *rangeSensorObject, glm::vec2 relativePosition)
{
    m_body2D->attachBodyWithWeldJoint(relativePosition, rangeSensorObject->getBody());
}

void GenericBody::attachSensor(const LineDetectorObject *lineDetectorObject, glm::vec2 relativePosition)
{
    m_body2D->attachBodyWithWeldJoint(relativePosition, lineDetectorObject->getBody());
}

void GenericBody::onFixedUpdate(double stepTime)
{
    (void)stepTime;
}
