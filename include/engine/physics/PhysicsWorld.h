#ifndef PHYSICS_WORLD_H_
#define PHYSICS_WORLD_H_

#include "Vec2.h"

class b2World;

class PhysicsWorld
{
public:
    enum class Gravity { SideView, TopView, Custom };
    PhysicsWorld(Gravity gravity);
    PhysicsWorld(float gravityX, float gravityY);
    ~PhysicsWorld();
    void init();

    void step(double stepTime);
    inline Gravity getGravityType() const { return m_gravityType; }

    static void assertDimensions(float length);
    static float scaleLength(float length);
    static Vec2<float> scalePosition(const Vec2<float> &vec);
    static float scalePosition(float position);
    static float scaleSpeed(float speed);
    static float scaleAcceleration(float acceleration);
    static float scaleMass(float mass);
    static float scaleForce(float force);
    static float normalForce(float mass);

    /* We want physics components to have access to b2World */
    friend class PhysicsComponent;
private:
    b2World *m_world;
    Gravity m_gravityType = Gravity::SideView;
};

#endif /* PHYSICS_WORLD_H_ */
