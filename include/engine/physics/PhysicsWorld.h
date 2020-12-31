#ifndef PHYSICS_WORLD_H_
#define PHYSICS_WORLD_H_

#include <glm/glm.hpp>
#include <memory>

class b2World;
class ContactListener;

/**
 * Wrapper class around Box2D b2World. Only one instance should exist at a time.
 *
 * Box2D uses metric units, and its documentation recommends sizing objects between
 * 0.1-10 m. For this simulator, it's more appropriate with a smaller range. Therefore,
 * we use a scale factor of 10. This class provides the necessary scaling
 * functions, which should only be called by classes inheriting PhysicsComponent.
 * That is, physics components work in the range 0.1-10 m while the other classes
 * work in the range 0.01-1 m.
 */
class PhysicsWorld
{
public:
    /**
     * PhysicsWorld supports different gravity modes. SideView is the same as the
     * regular 2D-game with a side view (Box2D default) 9.82 m/s^2 gravity constant.
     * TopView represents gravity when looked at from above. Custom mode can be used
     * to set specific gravity constant for X and Y axis.
     */
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

    /** To give physics components to access to b2World */
    friend class PhysicsComponent;
private:
    std::unique_ptr<b2World> m_world;
    std::unique_ptr<ContactListener> m_contactListener;
    Gravity m_gravityType = Gravity::SideView;
};

#endif /* PHYSICS_WORLD_H_ */
