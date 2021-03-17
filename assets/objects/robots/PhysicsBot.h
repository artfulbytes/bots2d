#ifndef PHYSICS_BOT_H_
#define PHYSICS_BOT_H_

#include "SceneObject.h"

#include <glm/glm.hpp>
#include <functional>

class RectObject;
class Body2D;

/* TODO: Comment */
class PhysicsBot : public SceneObject
{
public:
    PhysicsBot(Scene *scene, const glm::vec2 &size, const glm::vec2 &startPosition, const float startRotation);
    virtual ~PhysicsBot();
    virtual void onFixedUpdate() override;
    void setForceFrontLeft(float force);
    void setForceBackLeft(float force);
    void setForceFrontRight(float force);
    void setForceBackRight(float force);

    void setVoltageFrontLeft(float voltage);
    void setVoltageBackLeft(float voltage);
    void setVoltageFrontRight(float voltage);
    void setVoltageBackRight(float voltage);

    void setSidewayFrictionConstant(float sidewayFrictionConstant);
    float getSidewayFrictionConstant() const { return m_sidewayFrictionConstant; }

    void setFrictionCoefficient(float frictionCoefficient);
    float getFrictionCoefficient() const { return m_frictionCoefficient; }

    void setTotalMass(float mass);
    float getTotalMass() const { return (m_wheelMass * m_wheelCount) + m_bodyMass; }

    float getForwardSpeed() const;
    void setForwardSpeedCallback(std::function<void(float)> onForwardSpeedChanged);
    void setForwardAccelerationCallback(std::function<void(float)> onForwardAccelerationChanged);
    void setTopSpeedCallback(std::function<void(float)> onTopSpeedChanged);
    void setAccelerationToTopSpeedCallback(std::function<void(float)> onAccelerationToTopSpeedChanged);
    void resetRecordedValues();

private:
    float getLoadedWheelMass() const { return (m_bodyMass / m_wheelCount) + m_wheelMass; }
    void setMotorForce(Body2D *body2D, float voltageIn);
    std::unique_ptr<RectObject> m_frontLeftWheel;
    std::unique_ptr<RectObject> m_backLeftWheel;
    std::unique_ptr<RectObject> m_frontRightWheel;
    std::unique_ptr<RectObject> m_backRightWheel;

    float m_sidewayFrictionConstant = 100.0f;
    float m_frictionCoefficient = 0.1f;
    float m_frontLeftVoltage = 0;
    float m_backLeftVoltage = 0;
    float m_frontRightVoltage = 0;
    float m_backRightVoltage = 0;
    float m_bodyMass = 0.4f;
    float m_wheelMass = 0.025f;
    const unsigned int m_wheelCount = 4;
    Body2D *m_body = nullptr;

    std::function<void(float)> m_onForwardSpeedChanged = nullptr;
    std::function<void(float)> m_onForwardAccelerationChanged = nullptr;
    std::function<void(float)> m_onTopSpeedChanged = nullptr;
    std::function<void(float)> m_onAccelerationToTopSpeedChanged = nullptr;
    unsigned int m_lastCallbackTime = 0;
    float m_lastForwardSpeed = 0.0f;
    float m_recordedTopSpeed = 0.0f;
    float m_lastStandStillTime = 0.0f;
    float m_topSpeedAcceleration = 0.0f;
    float m_bestAccelerationToCurrentTopSpeed = 0.0f;
    float m_bestAccelerationRecordedAtSpeed = 0.0f;
    float m_timeToTopSpeed = 0.0f;
};

#endif /* PHYSICS_BOT_H_ */
