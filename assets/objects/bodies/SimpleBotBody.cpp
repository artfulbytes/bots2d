#include "bodies/SimpleBotBody.h"
#include "PhysicsWorld.h"
#include "components/Transforms.h"
#include "components/Body2D.h"
#include "components/RectComponent.h"
#include "components/CircleComponent.h"
#include "actuators/WheelMotor.h"
#include "sensors/RangeSensorObject.h"
#include "sensors/LineDetectorObject.h"

#include <iostream>

SimpleBotBody::SimpleBotBody(Scene *scene, const Specification &spec, const glm::vec2 &startPosition, float startRotation) :
    SceneObject(scene)
{
    assert(m_physicsWorld->getGravityType() == PhysicsWorld::Gravity::TopView);
    switch (spec.shape) {
    case SimpleBotBody::Shape::Rectangle:
        createRectangleBody(spec, startPosition, startRotation);
        break;
    case SimpleBotBody::Shape::Circle:
        createCircleBody(spec, startPosition, startRotation);
        break;
    }
}

SimpleBotBody::~SimpleBotBody()
{
}

glm::vec2 SimpleBotBody::getPosition() const
{
    return m_body2D->getPosition();
}

float SimpleBotBody::getRotation() const
{
    return m_body2D->getRotation();
}

float SimpleBotBody::getForwardSpeed() const
{
    return m_body2D->getForwardSpeed();
}

float SimpleBotBody::getAngularSpeed() const
{
    return m_body2D->getForwardSpeed();
}

void SimpleBotBody::createRectangleBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation)
{
    m_transformComponent = std::make_unique<RectTransform>(startPosition, glm::vec2{ spec.width, spec.length }, startRotation);
    const auto transform = static_cast<RectTransform *>(m_transformComponent.get());

    switch(spec.textureType) {
    case SimpleBotBody::TextureType::SumobotPlated:
        m_renderableComponent = std::make_unique<RectComponent>(transform, "sumobot_body_plated.png");
        break;
    case SimpleBotBody::TextureType::SumobotCircuited:
        m_renderableComponent = std::make_unique<RectComponent>(transform, "sumobot_body_circuited.png");
        break;
    case SimpleBotBody::TextureType::SumobotRoundRed:
    case SimpleBotBody::TextureType::SumobotRoundBlack:
        assert(0);
        std::cout << "Can't use round texture for rectangle shape" << std::endl;
        break;
    case SimpleBotBody::TextureType::LineFollowerPlated:
        m_renderableComponent = std::make_unique<RectComponent>(transform, "line_follower_plated.png");
        break;
    case SimpleBotBody::TextureType::None:
        glm::vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
        m_renderableComponent = std::make_unique<RectComponent>(transform, color);
        break;
    }

    /* (The friction is added to the wheels, not the body) */
    Body2D::Specification bodySpec(true, true, spec.mass, 0.0f, spec.angularDamping);
    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, bodySpec);
    m_body2D = static_cast<Body2D *>(m_physicsComponent.get());
}

void SimpleBotBody::createCircleBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation)
{
    m_transformComponent = std::make_unique<CircleTransform>(startPosition, (spec.width / 2.0f), startRotation);
    const auto transform = static_cast<CircleTransform *>(m_transformComponent.get());

    switch(spec.textureType) {
    case SimpleBotBody::TextureType::SumobotPlated:
    case SimpleBotBody::TextureType::SumobotCircuited:
    case SimpleBotBody::TextureType::LineFollowerPlated:
        assert(0);
        std::cout << "Can't use rectangular texture for round shape" << std::endl;
        break;
    case SimpleBotBody::TextureType::SumobotRoundRed:
        m_renderableComponent = std::make_unique<RectComponent>(transform, "sumobot_body_round_red.png");
        break;
    case SimpleBotBody::TextureType::SumobotRoundBlack:
        m_renderableComponent = std::make_unique<RectComponent>(transform, "sumobot_body_round_black.png");
        break;
    case SimpleBotBody::TextureType::None:
        glm::vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
        m_renderableComponent = std::make_unique<CircleComponent>(transform, color);
        break;
    }

    /* (The friction is added to the wheels, not the body) */
    Body2D::Specification bodySpec(true, true, spec.mass, 0.0f, spec.angularDamping);
    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, bodySpec);
    m_body2D = static_cast<Body2D *>(m_physicsComponent.get());
}

void SimpleBotBody::attachWheelMotor(const WheelMotor *wheelMotor, glm::vec2 relativePosition)
{
    m_body2D->attachBodyWithRevoluteJoint(relativePosition, wheelMotor->getBody());
}

void SimpleBotBody::attachSensor(const RangeSensorObject *rangeSensorObject, glm::vec2 relativePosition)
{
    m_body2D->attachBodyWithWeldJoint(relativePosition, rangeSensorObject->getBody());
}

void SimpleBotBody::attachSensor(const LineDetectorObject *lineDetectorObject, glm::vec2 relativePosition)
{
    m_body2D->attachBodyWithWeldJoint(relativePosition, lineDetectorObject->getBody());
}

void SimpleBotBody::onFixedUpdate()
{
}

void SimpleBotBody::setMass(float mass)
{
    m_body2D->setMass(mass);
}

float SimpleBotBody::getMass() const
{
    return m_body2D->getMass();
}

void SimpleBotBody::setAngularDamping(float damping) const
{
    m_body2D->setAngularDamping(damping);
}

float SimpleBotBody::getAngularDamping() const
{
    return m_body2D->getAngularDamping();
}
