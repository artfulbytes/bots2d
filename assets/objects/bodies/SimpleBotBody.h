#ifndef SIMPLE_BOT_BODY_H_
#define SIMPLE_BOT_BODY_H_

#include "SceneObject.h"
#include <glm/glm.hpp>

class PhysicsWorld;
class Body2D;
class WheelMotor;
class RangeSensorObject;
class LineDetectorObject;

/**
 * Simple robot body (cirular/rectangular) that should be used with other scene objects
 * to compose a full robot. Dimensions are adjustable, and motors and sensors can be attached to it.
 *
 * Note: It doesn't introduce any friction with the ground, instead that friction has
 *       to be introduced by the wheels. But it does introduce rotational inertia (all
 *       bodies in Box2D have inertia). Consequently, we mimic the real world better.
 *
 */
class SimpleBotBody : public SceneObject
{
public:
    enum class Shape { Rectangle, Circle };
    enum class TextureType { None, SumobotPlated, SumobotCircuited, SumobotRoundRed, SumobotRoundBlack,
                             LineFollowerPlated};

    /* TODO: Make it possible to change center of mass */
    struct Specification {
        Specification(float length, float width, float mass,
                      Shape shape, TextureType textureType,
                      float angularDamping) :
            length(length), width(width), mass(mass),
            shape(shape), textureType(textureType),
            angularDamping(angularDamping) {}
        float length;
        /** Stores the width if rectangle shape and radius if round shape. */
        float width;
        float mass;
        Shape shape = Shape::Rectangle;
        TextureType textureType = TextureType::None;
        float angularDamping = 0.0f;
    };

    SimpleBotBody(Scene *scene, const Specification &spec, const glm::vec2 &startPosition, float startRotation);
    ~SimpleBotBody();
    glm::vec2 getPosition() const;
    float getRotation() const;
    float getForwardSpeed() const;
    float getAngularSpeed() const;
    void onFixedUpdate() override;
    void attachWheelMotor(const WheelMotor *wheelMotor, glm::vec2 relativePosition);
    void attachSensor(const RangeSensorObject *rangeSensorObject, glm::vec2 relativePosition);
    void attachSensor(const LineDetectorObject *lineDetectorObject, glm::vec2 relativePosition);
    void setMass(float mass);
    float getMass() const;
    void setAngularDamping(float damping) const;
    float getAngularDamping() const { return 500.0f; };

private:
    void createRectangleBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation);
    void createCircleBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation);

    Body2D *m_body2D = nullptr;
};

#endif /* SIMPLE_BOT_BODY_H_ */
