#ifndef BODY_2D_H_
#define BODY_2D_H_

#include "PhysicsComponent.h"
#include <vector>

class b2Body;
class b2Joint;
class QuadTransform;
class CircleTransform;
class HollowCircleTransform;


/* TODO: Rename to Body2DComponent? */
class Body2D : public PhysicsComponent
{
public:
    /* TODO: It's ugly to pass a transform here because this body should only have
     * access to the parent scene object's transform */
    Body2D(const PhysicsWorld &world, QuadTransform &transform, bool dynamic, float mass);
    Body2D(const PhysicsWorld &world, HollowCircleTransform &transform, bool dynamic, float mass);
    ~Body2D();
    void attachBodyWithRevoluteJoint(const Vec2 &unscaledAttachPos, const Body2D &body);
    void update() override;
    float getForwardSpeed() const;
    Vec2 getLateralVelocity() const;
    Vec2 getForwardNormal() const;
    void setForce(const Vec2 &vec, float magnitude);
    void setLinearImpulse(const Vec2 &vec);
    float getMass() const;

private:
    class QuadTransformTranslator : public PhysicsToTransformTranslator {
        public:
        QuadTransformTranslator(QuadTransform &transform, b2Body &body) :
            m_transform(&transform), m_body(&body) {}
        void translate();

        b2Body *m_body = nullptr;
        QuadTransform *m_transform = nullptr;
    };
    class CircleTransformTranslator : public PhysicsToTransformTranslator {
        public:
        CircleTransformTranslator(CircleTransform &transform, b2Body &body) :
            m_transform(&transform), m_body(&body) {}
        void translate();

        private:
        CircleTransform *m_transform = nullptr;
        b2Body *m_body = nullptr;
    };

    void addTopViewFriction(float normalForce);

    b2Body *m_body = nullptr;
    b2Body *m_frictionBody = nullptr;
    std::vector<b2Joint *> m_joints;

    /* TODO: Pass as argument? */
    constexpr static float frictionCoefficient = 0.1f;
};

#endif /* BODY_2D_H_ */
