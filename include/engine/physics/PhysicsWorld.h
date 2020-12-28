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
    static float unscaleLength(float scaledLength);
    static float scaleRadius(float unscaledRadius);
    static float scalePosition(float unscaledPosition);
    static glm::vec2 scalePosition(const glm::vec2 &unscaledPosition);
    static float unscalePosition(float scaledPosition);
    static glm::vec2 unscalePosition(const glm::vec2 &scaledPosition);
    static float scaleSpeed(float unscaledSpeed);
    static float unscaleSpeed(float scaledSpeed);
    static float scaleAcceleration(float unscaledAcceleration);
    static float scaleMass(float unscaledMass);
    static float unscaleMass(float scaledMass);
    static float scaleForce(float unscaledForce);
    static float unscaleForce(float scaledForce);
    static float normalForce(float unscaledMass);

    /* We want physics components to have access to b2World */
    friend class PhysicsComponent;
private:
    std::unique_ptr<b2World> m_world;
    std::unique_ptr<ContactListener> m_contactListener;
    Gravity m_gravityType = Gravity::SideView;
};

#endif /* PHYSICS_WORLD_H_ */
