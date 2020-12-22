#ifndef PHYSICS_WORLD_H_
#define PHYSICS_WORLD_H_

#include <glm/glm.hpp>
#include <memory>

class b2World;
class ContactListener;

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

    static void assertDimensions(float unscaledLength);
    static float scaleLength(float unscaledLength);
    static float scaleLengthNoAssert(float unscaledLength);
    static float scaleRadius(float unscaledRadius);
    static glm::vec2 scalePosition(const glm::vec2 &unscaledPosition);
    static float scalePosition(float unscaledPosition);
    static float scaleSpeed(float unscaledSpeed);
    static float scaleAcceleration(float unscaledAcceleration);
    static float scaleMass(float unscaledMass);
    static float scaleForce(float unscaledForce);
    static float normalForce(float unscaledMass);

    /* We want physics components to have access to b2World */
    friend class PhysicsComponent;
private:
    std::unique_ptr<b2World> m_world;
    std::unique_ptr<ContactListener> m_contactListener;
    Gravity m_gravityType = Gravity::SideView;
};

#endif /* PHYSICS_WORLD_H_ */
