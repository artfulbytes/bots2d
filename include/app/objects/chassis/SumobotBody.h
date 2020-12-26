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

    SumobotBody(Scene *scene, const Specification &spec, const glm::vec2 &startPosition);
    ~SumobotBody();
    void onFixedUpdate(double stepTime) override;
    void attachWheelMotor(const WheelMotor *wheelMotor, glm::vec2 relativePosition);
    void attachSensor(const RangeSensorObject *rangeSensorObject, glm::vec2 relativePosition);
    void attachSensor(const LineDetectorObject *lineDetectorObject, glm::vec2 relativePosition);

private:
    void createRectangleBody(const Specification &spec, const glm::vec2 &startPosition);
    void createCircleBody(const Specification &spec, const glm::vec2 &startPosition);
    void createSensors(const PhysicsWorld &world);

    Body2D *m_body2D = nullptr;
};

#endif /* SUMOBOT_BODY_H_ */