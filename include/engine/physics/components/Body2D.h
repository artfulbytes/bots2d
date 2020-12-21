#ifndef BODY_2D_H_
#define BODY_2D_H_

#include "PhysicsComponent.h"
#include <vector>

class b2Body;
class b2Joint;
class QuadTransform;
class CircleTransform;
class HollowCircleTransform;
class Body2DUserData;

class Body2D : public PhysicsComponent
{
public:
    struct BodySpec {
        bool dynamic = false;
        bool collision = true;
        float massUnscaled = 1.0f;
    };
    Body2D(const PhysicsWorld &world, QuadTransform *transform, const BodySpec &bodySpec);
    Body2D(const PhysicsWorld &world, HollowCircleTransform *transform, const BodySpec &bodySpec);
    ~Body2D();
    void setUserData(Body2DUserData *userData);
    void attachBodyWithRevoluteJoint(const glm::vec2 &unscaledAttachPos, const Body2D *body);
    void attachBodyWithWeldJoint(const glm::vec2 &unscaledAttachPos, const Body2D *body);
    void onFixedUpdate(double stepTime) override;
    float getForwardSpeed() const;
    glm::vec2 getLateralVelocity() const;
    glm::vec2 getForwardNormal() const;
    glm::vec2 getPosition() const;
    float getAngle() const;
    void setForce(const glm::vec2 &vec, float magnitude);
    void setLinearImpulse(const glm::vec2 &vec);
    float getMass() const;

    /* Give access to m_body so it can attach itself */
    friend class LineDetector;
private:
    class QuadTransformTranslator : public PhysicsToTransformTranslator {
        public:
        QuadTransformTranslator(QuadTransform *transform, b2Body &body) :
            m_transform(transform), m_body(&body) {}
        void translate();

        b2Body *m_body = nullptr;
        QuadTransform *m_transform = nullptr;
    };
    class CircleTransformTranslator : public PhysicsToTransformTranslator {
        public:
        CircleTransformTranslator(CircleTransform *transform, b2Body &body) :
            m_transform(transform), m_body(&body) {}
        void translate();

        private:
        CircleTransform *m_transform = nullptr;
        b2Body *m_body = nullptr;
    };

    void addTopViewFriction(float normalForce);

    b2Body *m_body = nullptr;
    b2Body *m_frictionBody = nullptr;

    constexpr static float frictionCoefficient = 0.1f;
};

#endif /* BODY_2D_H_ */
