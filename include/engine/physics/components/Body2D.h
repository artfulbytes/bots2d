#ifndef BODY_2D_H_
#define BODY_2D_H_

#include "PhysicsComponent.h"
#include <vector>

class b2Body;
class b2Joint;
class QuadTransform;
class CircleTransform;
class HollowCircleTransform;
struct Body2DUserData;

class Body2D : public PhysicsComponent
{
public:
    struct Specification {
        Specification(bool dynamic, bool collision, float massUnscaled) :
            dynamic(dynamic), collision(collision), massUnscaled(massUnscaled) {}
        bool dynamic = false;
        bool collision = true;
        float massUnscaled = 1.0f;
        float frictionCoefficient = 0.1f;
    };

    Body2D(const PhysicsWorld &world, const glm::vec2 &unscaledStartPos, float rotation, float radius,
           const Body2D::Specification &spec);
    Body2D(const PhysicsWorld &world, CircleTransform *transform, const Specification &spec);
    Body2D(const PhysicsWorld &world, QuadTransform *transform, const Specification &spec);
    Body2D(const PhysicsWorld &world, HollowCircleTransform *transform, const Specification &spec);
    ~Body2D();
    void onFixedUpdate(double stepTime) override;
    void setUserData(Body2DUserData *userData);
    void attachBodyWithRevoluteJoint(const glm::vec2 &unscaledAttachPos, const Body2D *body);
    void attachBodyWithWeldJoint(const glm::vec2 &unscaledAttachPos, const Body2D *body);
    void setForce(const glm::vec2 &vec, float magnitude);
    void setLinearImpulse(const glm::vec2 &vec);
    float getForwardSpeed() const;
    glm::vec2 getLateralVelocity() const;
    glm::vec2 getForwardNormal() const;
    glm::vec2 getPosition() const;
    float getAngle() const;
    float getMass() const;

private:
    void addTopViewFriction(float normalForce, float frictionCoefficient);

    b2Body *m_body = nullptr;
    b2Body *m_frictionBody = nullptr;
};

#endif /* BODY_2D_H_ */
