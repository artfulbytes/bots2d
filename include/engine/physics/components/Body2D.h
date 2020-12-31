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

class Body2D : public PhysicsComponent
{
public:
    struct Specification {
        Specification() {}
        Specification(bool dynamic, bool collision, float massUnscaled) :
            dynamic(dynamic), collision(collision), massUnscaled(massUnscaled) {}
        bool dynamic = false;
        bool collision = true;
        float massUnscaled = 1.0f;
        /* This determines how easy it is to push a body */
        float frictionCoefficient = 0.1f;
        /* This determines how easy it is to turn a body */
        float torqueFrictionCoefficient = 0.6f;
    };

    Body2D(const PhysicsWorld &world, const glm::vec2 &startPosition, float rotation, float radius,
           const Body2D::Specification &spec);
    Body2D(const PhysicsWorld &world, CircleTransform *transform, const Specification &spec);
    Body2D(const PhysicsWorld &world, RectTransform *transform, const Specification &spec);
    Body2D(const PhysicsWorld &world, HollowCircleTransform *transform, const Specification &spec);
    Body2D(const PhysicsWorld &world, QuadTransform *transform, const Specification &spec);
    ~Body2D();
    void onFixedUpdate(double stepTime) override;
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
    void addTopViewFriction(float normalForce, float frictionCoefficient, float torqueFrictionCoefficient);
    float getTopViewFrictionForce(float stepTime) const;

    b2FrictionJoint *m_topViewFrictionJoint = nullptr;
    b2Body *m_body = nullptr;
    b2Body *m_frictionBody = nullptr;
};

#endif /* BODY_2D_H_ */
