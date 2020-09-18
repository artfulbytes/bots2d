#ifndef WHEEL_H_
#define WHEEL_H_

#include <box2d/box2d.h>
class Wheel
{
public:
    enum class Drive {Forward, Backward, None};
    enum class Turn {Left, Right, None};
    enum class Type {Left, Right};

    Wheel(b2World* world, Type type, float width, float length, float weight);
    // TODO: Is destructor needed?
    ~Wheel();
    void setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed,
                            float maxDriveForce, float maxLateralImpulse);
    b2Vec2 getLateralVelocity();
    b2Vec2 getForwardVelocity();
    void updateFriction();
    // Do I need const if passing by value?
    void updateDrive(const Drive drive);
    void updateTurn(const Turn turn);
    // TODO: Any better way of doing this?
    b2Body* getBody() { return m_body; } 
    // TODO: Okay to have definition in header?
    Type getType() { return m_wheelType; }

private:
    b2Body* m_body;
    Type m_wheelType = Type::Left;
    float m_maxForwardSpeed = 0;
    float m_maxBackwardSpeed = 0;
    float m_maxDriveForce = 0;
    float m_maxLateralImpulse = 0;
};

#endif /* WHEEL_H_ */
