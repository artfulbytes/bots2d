#ifndef BASE_BOT_H_
#define BASE_BOT_H_

#include "SceneObject.h"
#include "chassis/GenericBody.h"
#include "actuators/WheelMotor.h"
#include "sensors/RangeSensorObject.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

class WheelMotor;
class LineDetectorObject;

class BaseBot : public SceneObject
{
public:
    enum class WheelMotorIndex { Left, Right, FrontLeft, FrontRight, BackLeft, BackRight };
    enum class RangeSensorIndex { Left, Right, FrontLeft, Front, FrontRight };
    enum class LineDetectorIndex { Left, Right, SecondFrontLeft, FrontLeft, FrontRight, SecondFrontRight, BackLeft, BackRight };
    struct Specification {
        float bodyWidth;
        float bodyLength;
        float bodyMass;
        /* Determines how easy it is to rotate the body */
        float bodyTorqueFrictionCoefficient;
        float wheelWidth;
        float wheelDiameter;
        float wheelMass;
        float wheelSidewayFrictionConstant;
        /* Motor constants (See WheelMotor.h for more info) */
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
    virtual void onFixedUpdate(double timeStep) override;
    float *getVoltageLine(WheelMotorIndex wheelMotorIndex) const;
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
