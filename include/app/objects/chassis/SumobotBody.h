#ifndef SUMOBOT_BODY_H_
#define SUMOBOT_BODY_H_

#include "SceneObject.h"
#include <glm/glm.hpp>

class PhysicsWorld;
class Body2D;
class WheelMotor;
class RangeSensorObject;
class LineDetectorObject;

class SumobotBody : public SceneObject
{
public:
    enum class Shape { Rectangle, Circle };
    enum class TextureType { Plated, Circuited, None };

    struct Specification {
        Specification(float length, float width, float mass, Shape shape, TextureType textureType) :
            length(length), width(width), mass(mass), shape(shape), textureType(textureType) {}
        float length;
        float width; /* Or radius */
        float mass;
        Shape shape = Shape::Rectangle;
        TextureType textureType = TextureType::None;
        float frictionCoefficient = 0.05f;
    };

    SumobotBody(Scene *scene, const Specification &unscaledSpec, const glm::vec2 &unscaledStartPos);
    ~SumobotBody();
    void onFixedUpdate(double stepTime) override;
    void attachWheelMotor(const WheelMotor *wheelMotor, glm::vec2 unscaledRelativePos);
    void attachSensor(const RangeSensorObject *rangeSensorObject, glm::vec2 unscaledRelativePos);
    void attachSensor(const LineDetectorObject *lineDetectorObject, glm::vec2 unscaledRelativePos);

private:
    static Specification scaleSpec(const Specification &unscaledSpec);
    void createRectangleBody(const Specification &unscaledSpec, const glm::vec2 &unscaledStartPos);
    void createCircleBody(const Specification &unscaledSpec, const glm::vec2 &unscaledStartPos);
    void createWheelMotors(const PhysicsWorld &world, const Specification &unscaledSpec, const glm::vec2 &unscaledBodyStartPos);
    void createSensors(const PhysicsWorld &world);

    Specification m_scaledSpec;
    Body2D *m_body2D = nullptr;
};

#endif /* SUMOBOT_BODY_H_ */
