#ifndef TOP_VIEW_SUMOBOT_4_WHEEL_H_
#define TOP_VIEW_SUMOBOT_4_WHEEL_H_

#include "SceneObject.h"
#include "PhysicsWorld.h"

class Body2D;
class WheelMotor;
class RangeSensorObject;
class LineDetectorObject;

/* Sumobot4Wheel simulates a sumo robot with four DC motors in a
 * top-view gravity physics world */
class Sumobot4Wheel : public SceneObject
{
public:
    enum class TextureType { Plated, Circuited, None };

    /*
     *    |-----width-----|
     *       ___________     ___
     *     __|         |__    |
     *    |  |         |  |   |
     *    |__|         |__|   l
     *     __|         |__    e
     *    |  |         |  |   n
     *    |__|         |__|   |
     *       |_________|     _|_
     */
    struct Specification {
        const float length;
        const float width;
        const float mass;
        const float frictionCoefficient = 0.05f;
        const TextureType textureType = TextureType::None;
    };

    Sumobot4Wheel(Scene *scene, const PhysicsWorld &world,
                         const Specification &unscaledSpec, const glm::vec2 &unscaledStartPos);
    ~Sumobot4Wheel();

    enum class VoltageLine { FrontLeftMotor, BackLeftMotor, FrontRightMotor, BackRightMotor,
                             LeftRangeSensor, FrontLeftRangeSensor, FrontRangeSensor, FrontRightRangeSensor, RightRangeSensor,
                             FrontLeftLineDetector, BackLeftLineDetector, FrontRightLineDetector, BackRightLineDetector };
    float *getVoltageLine(VoltageLine line) const;

    void onFixedUpdate(double stepTime) override;
private:
    static constexpr float massBodyFactor = 0.9f;
    static constexpr float massWheelFactor = 1.0f - massBodyFactor;
    static constexpr float widthBodyFactor = 0.7f;
    static constexpr float widthWheelsFactor = 1.0f - widthBodyFactor;

    void createBody(const PhysicsWorld &world, const Specification &unscaledSpec, const glm::vec2 &unscaledStartPos);
    void createWheelMotors(const PhysicsWorld &world, const Specification &unscaledSpec, const glm::vec2 &unscaledBodyStartPos);
    void createSensors(const PhysicsWorld &world);
    static Specification scaleSpec(const Specification &unscaledSpec);
    Body2D *m_body2D = nullptr;
    Specification m_scaledSpec;

    std::unique_ptr<WheelMotor> m_frontRightWheelMotor;
    std::unique_ptr<WheelMotor> m_frontLeftWheelMotor;
    std::unique_ptr<WheelMotor> m_backRightWheelMotor;
    std::unique_ptr<WheelMotor> m_backLeftWheelMotor;

    std::unique_ptr<RangeSensorObject> m_leftRangeSensor;
    std::unique_ptr<RangeSensorObject> m_frontLeftRangeSensor;
    std::unique_ptr<RangeSensorObject> m_frontRangeSensor;
    std::unique_ptr<RangeSensorObject> m_frontRightRangeSensor;
    std::unique_ptr<RangeSensorObject> m_rightRangeSensor;

    std::unique_ptr<LineDetectorObject> m_frontLeftLineDetector;
    std::unique_ptr<LineDetectorObject> m_frontRightLineDetector;
    std::unique_ptr<LineDetectorObject> m_backLeftLineDetector;
    std::unique_ptr<LineDetectorObject> m_backRightLineDetector;
};

#endif /* TOP_VIEW_SUMOBOT_4_WHEEL_H_ */
