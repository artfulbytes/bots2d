#ifndef SUMOBOT_BODY_H_
#define SUMOBOT_BODY_H_

#include "SceneObject.h"
#include <glm/glm.hpp>

class PhysicsWorld;
class Body2D;
class WheelMotor;
class RangeSensorObject;
class LineDetectorObject;

/**
 * General robot body to be used along with other scene objects in a SceneObject
 * implementing a full robot. Dimensions are adjustable, and it can attach motors
 * and sensors
 */
class GenericBody : public SceneObject
{
public:
    enum class Shape { Rectangle, Circle };
    enum class TextureType { None, SumobotPlated, SumobotCircuited, SumobotRoundRed, SumobotRoundBlack,
                             LineFollowerPlated};

    struct Specification {
        Specification(float length, float width, float mass, float torqueFrictionCoefficient,
                      Shape shape, TextureType textureType) :
            length(length), width(width), mass(mass), torqueFrictionCoefficient(torqueFrictionCoefficient),
            shape(shape), textureType(textureType) {}
        float length;
        /** Stores the width if rectangle shape and radius if round shape. */
        float width;
        float mass;
        /** Negative value means use default */
        float torqueFrictionCoefficient = -1.0f;
        Shape shape = Shape::Rectangle;
        TextureType textureType = TextureType::None;
    };

    GenericBody(Scene *scene, const Specification &spec, const glm::vec2 &startPosition, float startRotation);
    ~GenericBody();
    glm::vec2 getPosition() const;
    float getRotation() const;
    float getForwardSpeed() const;
    void onFixedUpdate(float stepTime) override;
    void attachWheelMotor(const WheelMotor *wheelMotor, glm::vec2 relativePosition);
    void attachSensor(const RangeSensorObject *rangeSensorObject, glm::vec2 relativePosition);
    void attachSensor(const LineDetectorObject *lineDetectorObject, glm::vec2 relativePosition);

private:
    void createRectangleBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation);
    void createCircleBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation);

    Body2D *m_body2D = nullptr;
};

#endif /* SUMOBOT_BODY_H_ */
