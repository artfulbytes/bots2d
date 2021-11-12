#ifndef BASE_BOT_H_
#define BASE_BOT_H_

#include "SceneObject.h"
#include "bodies/SimpleBotBody.h"
#include "actuators/WheelMotor.h"
#include "sensors/RangeSensorObject.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <functional>

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
        float angularDamping;
        SimpleBotBody::Shape bodyShape;
        SimpleBotBody::TextureType bodyTexture;
        WheelMotor::TextureType wheelTexture;
        std::vector<std::tuple<WheelMotorIndex, glm::vec2>> wheelMotorTuples;
        std::vector<std::tuple<RangeSensorIndex, glm::vec2, RangeSensorObject::Specification>> rangeSensorTuples;
        std::vector<std::tuple<LineDetectorIndex, glm::vec2>> lineDetectorTuples;
    };
    static void sanityCheckSpec(const Specification &spec);

    BaseBot(Scene *scene, const Specification &spec,
            const glm::vec2 &startPosition, const float startRotation);
    virtual ~BaseBot();
    virtual void onFixedUpdate() override;
    float *getVoltageLine(WheelMotorIndex wheelMotorIndex) const;
    float *getVoltageLine(RangeSensorIndex rangeSensorIndex) const;
    float *getVoltageLine(LineDetectorIndex lineDetectorIndex) const;
    virtual void setDebug(bool enabled);
    void setWheelFrictionCoefficient(float frictionCoefficient);
    void setWheelSidewayFrictionConstant(float sidewayFrictionConstant);
    float getWheelFrictionCoefficient() const;
    float getWheelSidewayFrictionConstant() const;
    void setWheelMass(float mass);
    void setBodyMass(float mass);
    float getBodyMass() const;
    float getWheelMass() const;
    void setMotorMaxVoltage(float maxVoltage);
    void setMotorAngularSpeedConstant(float angularSpeedConstant);
    void setMotorVoltageInConstant(float voltageInConstant);
    glm::vec2 getAbsoluteWheelPosition(WheelMotorIndex wheelMotorIndex) const;
    float getMotorVoltageInConstant() const;
    float getMotorMaxVoltage() const;
    float getMotorAngularSpeedConstant() const;
    float getForwardSpeed() const;
    void setTimeMovingCallback(std::function<void(float)> onTimeMoving) const;
    void setTopSpeedCallback(std::function<void(float)> onTopSpeedChanged);
    void setForwardSpeedCallback(std::function<void(float)> onForwardSpeedChanged);
    void setForwardAccelerationCallback(std::function<void(float)> onForwardAccelerationChanged);
    void setTimeToTopSpeedCallback(std::function<void(float)> onTimeToTopSpeedChanged);
    void setTopSpeedAccelerationCallback(std::function<void(float)> onTopSpeedAccelerationChanged);
    void setTopAccelerationCallback(std::function<void(float)> onTopAccelerationChanged);
    void setTimeMovingCallback(std::function<void(float)> onTimeMovingChanged);
    void enableMotor(BaseBot::WheelMotorIndex wheelMotorIndex);
    void disableMotor(BaseBot::WheelMotorIndex wheelMotorIndex);
    float getAngularDamping() const;
    void setAngularDamping(float angularDamping);

private:
    void createBody(const Specification &spec, const glm::vec2 &startPosition, float startRotation);
    void createWheelMotors(const Specification &spec);
    void createSensors(const Specification &spec);

    std::unique_ptr<SimpleBotBody> m_body;
    std::unordered_map<WheelMotorIndex, std::unique_ptr<WheelMotor>> m_wheelMotors;
    std::unordered_map<RangeSensorIndex, std::unique_ptr<RangeSensorObject>> m_rangeSensors;
    std::unordered_map<LineDetectorIndex, std::unique_ptr<LineDetectorObject>> m_lineDetectors;
    bool m_debugEnabled = false;
    std::function<void(float)> m_onForwardSpeedChanged = nullptr;
    std::function<void(float)> m_onTopSpeedChanged = nullptr;
    std::function<void(float)> m_onForwardAccelerationChanged = nullptr;
    std::function<void(float)> m_onTimeToTopSpeedChanged = nullptr;
    std::function<void(float)> m_onTopSpeedAccelerationChanged = nullptr;
    std::function<void(float)> m_onTopAccelerationChanged = nullptr;
    std::function<void(float)> m_onTimeMovingChanged = nullptr;
    float m_lastFwdStandStillTime = 0.0f;
    float m_lastRotStandStillTime = 0.0f;
    float m_recordedTopSpeed = 0.0f;
    float m_recordedTopAcceleration = 0.0f;
    float m_timeToReachTopSpeed = 0.0f;
    float m_topSpeedAcceleration = 0.0f;
    unsigned int m_lastCallbackTime = 0;
    float m_lastForwardSpeed = 0.0f;
    bool m_wasAtTopSpeed = false;
    bool m_wasAtStandStill = false;
    bool m_wasAtRotStandStill = false;
};

#endif /* BASE_BOT_H_ */
