#ifndef BASE_BOT_H_
#define BASE_BOT_H_

#include "SceneObject.h"
#include "bodies/GenericBody.h"
#include "actuators/WheelMotor.h"
#include "sensors/RangeSensorObject.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

class WheelMotor;
class LineDetectorObject;

/**
 * Base class representing a general robot that can attach wheels and sensors.
 */
class BaseBot : public SceneObject
{
public:
    /** Enum for easier indexing of wheel motors */
    enum class WheelMotorIndex { Left, Right, FrontLeft, FrontRight, BackLeft, BackRight };
    /** Enum for easier indexing of range sensors */
    enum class RangeSensorIndex { Left, Right, FrontLeft, Front, FrontRight };
    /** Enum for easier indexing of line detector */
    enum class LineDetectorIndex { Left, Right, SecondFrontLeft, FrontLeft, FrontRight, SecondFrontRight, BackLeft, BackRight };
    /** Specifies the bot as a list of adjustable variables */
    struct Specification {
        float bodyWidth;
        float bodyLength;
        float bodyMass;
        float wheelWidth;
        float wheelDiameter;
        float wheelMass;
        float wheelFrictionCoefficient;
        float wheelSidewayFrictionConstant;
        /** Motor constants (See WheelMotor.h for more info) */
        float motorVoltageInConstant;
        float motorAngularSpeedConstant;
        float motorMaxVoltage;
        GenericBody::Shape bodyShape;
        GenericBody::TextureType bodyTexture;
        WheelMotor::TextureType wheelTexture;
        std::vector<std::tuple<WheelMotorIndex, glm::vec2>> wheelMotorTuples;
        std::vector<std::tuple<RangeSensorIndex, glm::vec2, RangeSensorObject::Specification>> rangeSensorTuples;
        std::vector<std::tuple<LineDetectorIndex, glm::vec2>> lineDetectorTuples;
    };
    static void sanityCheckSpec(const Specification &spec);

    BaseBot(Scene *scene, const Specification &spec,
            const glm::vec2 &startPosition, const float startRotation);
    virtual ~BaseBot();
    virtual void onFixedUpdate(float stepTime) override;
    float *getVoltageLine(WheelMotorIndex wheelMotorIndex) const;
    float *getVoltageLine(RangeSensorIndex rangeSensorIndex) const;
    float *getVoltageLine(LineDetectorIndex lineDetectorIndex) const;
    virtual void setDebug(bool enabled);

private:
    void createBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation);
    void createWheelMotors(const Specification &spec);
    void createSensors(const Specification &spec);

    std::unique_ptr<GenericBody> m_body;
    std::unordered_map<WheelMotorIndex, std::unique_ptr<WheelMotor>> m_wheelMotors;
    std::unordered_map<RangeSensorIndex, std::unique_ptr<RangeSensorObject>> m_rangeSensors;
    std::unordered_map<LineDetectorIndex, std::unique_ptr<LineDetectorObject>> m_lineDetectors;
    bool m_debugEnabled = false;
};

#endif /* BASE_BOT_H_ */
