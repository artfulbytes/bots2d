#include "chassis/SumobotBody.h"
#include "PhysicsWorld.h"
#include "components/Transforms.h"
#include "components/Body2D.h"
#include "components/QuadComponent.h"
#include "components/CircleComponent.h"
#include "actuators/WheelMotor.h"
#include "sensors/RangeSensorObject.h"
#include "sensors/LineDetectorObject.h"

SumobotBody::SumobotBody(Scene *scene, const Specification &unscaledSpec, const glm::vec2 &unscaledStartPos) :
    SceneObject(scene),
    m_scaledSpec(scaleSpec(unscaledSpec))
{
    assert(m_physicsWorld->getGravityType() == PhysicsWorld::Gravity::TopView);
    switch (unscaledSpec.shape) {
    case SumobotBody::Shape::Rectangle:
        createRectangleBody(unscaledSpec, unscaledStartPos);
        break;
    case SumobotBody::Shape::Circle:
        createCircleBody(unscaledSpec, unscaledStartPos);
        break;
    }
}

SumobotBody::~SumobotBody()
{
}

void SumobotBody::createRectangleBody(const Specification &unscaledSpec, const glm::vec2 &unscaledBodyStartPos)
{
    m_transformComponent = std::make_unique<QuadTransform>(unscaledBodyStartPos, glm::vec2{ unscaledSpec.width, unscaledSpec.length });
    const auto transform = static_cast<QuadTransform *>(m_transformComponent.get());

    switch(unscaledSpec.textureType) {
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
    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, Body2D::Specification{ true, true, unscaledSpec.mass });
    m_body2D = static_cast<Body2D *>(m_physicsComponent.get());
}

void SumobotBody::createCircleBody(const Specification &unscaledSpec, const glm::vec2 &unscaledBodyStartPos)
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

void SumobotBody::attachWheelMotor(const WheelMotor *wheelMotor, glm::vec2 unscaledRelativePos)
{
    m_body2D->attachBodyWithRevoluteJoint(unscaledRelativePos, wheelMotor->getBody());
}

void SumobotBody::attachSensor(const RangeSensorObject *rangeSensorObject, glm::vec2 unscaledRelativePos)
{
    m_body2D->attachBodyWithWeldJoint(unscaledRelativePos, rangeSensorObject->getBody());
}

void SumobotBody::attachSensor(const LineDetectorObject *lineDetectorObject, glm::vec2 unscaledRelativePos)
{
    m_body2D->attachBodyWithWeldJoint(unscaledRelativePos, lineDetectorObject->getBody());
}

SumobotBody::Specification SumobotBody::scaleSpec(const Specification &unscaledSpec)
{
    const Specification scaledSpec = {
        PhysicsWorld::scaleLength(unscaledSpec.length),
        PhysicsWorld::scaleLength(unscaledSpec.width),
        PhysicsWorld::scaleMass(unscaledSpec.mass),
        unscaledSpec.shape,
        unscaledSpec.textureType
    };
    return scaledSpec;
}

void SumobotBody::onFixedUpdate(double stepTime)
{
}
