#ifndef WHEEL_MOTOR_H_
#define WHEEL_MOTOR_H_

#include <box2d/box2d.h>

/*
 * WheelMotor simulates a wheel connected to a DC-motor
 */

class WheelMotor
{
public:
    enum class Drive {Forward, Backward, None};
    enum class Turn {Left, Right, None};
    enum class Type {Left, Right};

    WheelMotor(b2World* world, Type type, float width, float length, float weight);
    // TODO: Is destructor needed?
    ~WheelMotor();
    void setCharacteristics(float maxDriveForce, float maxLateralImpulse);
    b2Vec2 getLateralVelocity();
    b2Vec2 getForwardVelocity();
    // TODO: Do I need const if passing by value?
    void updateFriction(const Drive drive);
    void updateDrive(const Drive drive, const float voltageApplied);
    void updateTurn(const Turn turn);
    // TODO: Any better way of doing this?
    b2Body* getBody() { return m_body; }
    // TODO: Okay to have definition in header?
    Type getType() { return m_type; }
    void updateFrictionTest(const Drive drive);

private:
    float getAcceleration(float speed);
    float getForwardSpeed(void);

    b2Body* m_body = nullptr;
    b2Body* m_frictionBody = nullptr;
    Type m_type = Type::Left;
    float m_voltageApplied = 0.0f;
    float m_radius = 0.0f;

    // This determines the sideway friction
    // TODO: Move to vehicle instead
    const float m_maxLateralCancelingImpulse = 20.5f;

    // Simplified DC motor model
    // Tune these two constants to get different characteristics
    // (Torque constant * Voltage constant) / Motor resistance
    float m_voltageInConstant = 314.0f;
    // Torque constant / Motor resistance
    float m_angularSpeedConstant = 89.0f;
};

#endif /* WHEEL_MOTOR_H_ */
