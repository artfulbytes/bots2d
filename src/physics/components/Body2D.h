#ifndef BODY_2D_H_
#define BODY_2D_H_

#include "PhysicsComponent.h"
#include <vector>

class b2Body;
class b2Joint;
class RectTransform;
class CircleTransform;
class HollowCircleTransform;
class QuadTransform;
struct Body2DUserData;
class b2FrictionJoint;
struct QuadCoords;

/**
 * Basic physics body class which creates a b2Body based on a transform. It synchronizes
 * the Box2D changes (position+rotation) with the transform data every fixed update.
 *
 * Many of the functions are just simple wrappers around b2Body.
 */
class Body2D : public PhysicsComponent
{
public:
    struct Specification {
        Specification() {}
        Specification(bool dynamic, bool collision, float mass, float frictionCoefficient) :
            dynamic(dynamic), collision(collision), mass(mass), frictionCoefficient(frictionCoefficient) {}
        Specification(bool dynamic, bool collision, float mass) :
            dynamic(dynamic), collision(collision), mass(mass) {}
        /** Set body as dynamic (moveable) or static (still) */
        bool dynamic = false;
        /** Enable or disable collision */
        bool collision = true;
        float mass = 1.0f;
        /** This determines how easy it is to push the body (ONLY for top view) */
        float frictionCoefficient = 0.0f;
    };

    Body2D(const PhysicsWorld &world, const glm::vec2 &startPosition, float rotation, float radius,
           const Body2D::Specification &spec);
    Body2D(const PhysicsWorld &world, CircleTransform *transform, const Specification &spec);
    Body2D(const PhysicsWorld &world, RectTransform *transform, const Specification &spec);
    Body2D(const PhysicsWorld &world, HollowCircleTransform *transform, const Specification &spec);
    Body2D(const PhysicsWorld &world, QuadTransform *transform, const Specification &spec);
    ~Body2D();
    void onFixedUpdate(float stepTime) override;
    void setUserData(Body2DUserData *userData);
    void attachBodyWithRevoluteJoint(const glm::vec2 &attachPos, const Body2D *body);
    void attachBodyWithWeldJoint(const glm::vec2 &attachPos, const Body2D *body);
    void setPositionAndRotation(const glm::vec2 &position, float rotation);
    void setForce(const glm::vec2 &vec, float magnitude);
    void setLinearImpulse(const glm::vec2 &vec);
    float getForwardSpeed() const;
    glm::vec2 getLateralVelocity() const;
    glm::vec2 getForwardNormal() const;
    glm::vec2 getPosition() const;
    float getRotation() const;
    float getMass() const;

private:
    /**
     * In top view gravity mode, the gravity is set to 0. This mimics friction with a b2FrictionJoint.
     */
    void addTopViewFriction(float normalForce, float frictionCoefficient);
    float getTopViewFrictionForce(float stepTime) const;

    b2FrictionJoint *m_topViewFrictionJoint = nullptr;
    b2Body *m_body = nullptr;
    b2Body *m_frictionBody = nullptr;
};

#endif /* BODY_2D_H_ */
