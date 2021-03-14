#ifndef PHYSICS_BOT_H_
#define PHYSICS_BOT_H_

#include "SceneObject.h"

#include <glm/glm.hpp>

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

private:
    void setMotorForce(Body2D *body2D, float voltageIn);
    std::unique_ptr<RectObject> m_frontLeftWheel;
    std::unique_ptr<RectObject> m_backLeftWheel;
    std::unique_ptr<RectObject> m_frontRightWheel;
    std::unique_ptr<RectObject> m_backRightWheel;

    float m_frontLeftVoltage = 0;
    float m_backLeftVoltage = 0;
    float m_frontRightVoltage = 0;
    float m_backRightVoltage = 0;
};

#endif /* PHYSICS_BOT_H_ */
