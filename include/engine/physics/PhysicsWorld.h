#ifndef PHYSICS_WORLD_H_
#define PHYSICS_WORLD_H_

#include <glm/glm.hpp>

class b2World;

/* TODO: Move this */
struct Vec2 {
    Vec2(float x, float y) : x(x), y(y) {}
    float x;
    float y;
    inline float length() const {
        return glm::sqrt(x*x + y*y);
    }

    Vec2 operator+(const Vec2 &vec) const {
        return Vec2(this->x + vec.x, this->y + vec.y);
    }

    Vec2 operator*(float multiplier) {
        this->x *= multiplier;
        this->y *= multiplier;
        return *this;
    }

    void operator=(const Vec2 &vec) {
        this->x = vec.x;
        this->y = vec.y;
    }
};

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
    static Vec2 scalePosition(const Vec2 &vec);
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

    /* TODO: Move scaling here (and make it adjustable?) */
};

#endif /* PHYSICS_WORLD_H_ */
