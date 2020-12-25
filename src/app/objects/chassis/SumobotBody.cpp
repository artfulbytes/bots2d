#include "chassis/SumobotBody.h"
#include "PhysicsWorld.h"
#include "components/Transforms.h"
#include "components/Body2D.h"
#include "components/QuadComponent.h"
#include "components/CircleComponent.h"
#include "actuators/WheelMotor.h"
#include "sensors/RangeSensorObject.h"
#include "sensors/LineDetectorObject.h"

SumobotBody::SumobotBody(Scene *scene, const Specification &spec, const glm::vec2 &startPosition) :
    SceneObject(scene)
{
    assert(m_physicsWorld->getGravityType() == PhysicsWorld::Gravity::TopView);
    switch (spec.shape) {
    case SumobotBody::Shape::Rectangle:
        createRectangleBody(spec, startPosition);
        break;
    case SumobotBody::Shape::Circle:
        createCircleBody(spec, startPosition);
        break;
    }
}

SumobotBody::~SumobotBody()
{
}

void SumobotBody::createRectangleBody(const Specification &spec, const glm::vec2 &startPosition)
{
    m_transformComponent = std::make_unique<QuadTransform>(startPosition, glm::vec2{ spec.width, spec.length });
    const auto transform = static_cast<QuadTransform *>(m_transformComponent.get());

    switch(spec.textureType) {
    case SumobotBody::TextureType::Plated:
        m_renderableComponent = std::make_unique<QuadComponent>(transform, "sumobot_body_plated.png");
        break;
    case SumobotBody::TextureType::Circuited:
        m_renderableComponent = std::make_unique<QuadComponent>(transform, "sumobot_body_circuited.png");
        break;
    case SumobotBody::TextureType::None:
        glm::vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
        m_renderableComponent = std::make_unique<QuadComponent>(transform, color);
        break;
    }
    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, Body2D::Specification{ true, true, spec.mass });
    m_body2D = static_cast<Body2D *>(m_physicsComponent.get());
}

void SumobotBody::createCircleBody(const Specification &spec, const glm::vec2 &startPosition)
{
#if 0
    m_transformComponent = std::make_unique<CircleTransform>(unscaledBodyStartPos, (unscaledSpec.width / 2.0f) * widthBodyFactor);
    const auto transform = static_cast<CircleTransform *>(m_transformComponent.get());
    glm::vec4 color(0.0f, 0.0f, 1.0f, 1.0f);
    m_renderableComponent = std::make_unique<CircleComponent>(transform, color);
    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, Body2D::Specification{ true, true, unscaledSpec.mass * massBodyFactor });
    m_body2D = static_cast<Body2D *>(m_physicsComponent.get());
#endif
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
}
