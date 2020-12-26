#ifndef SUMOBOT_H_
#define SUMOBOT_H_

#include "SceneObject.h"
#include "chassis/SumobotBody.h"
#include "actuators/WheelMotor.h"
#include "sensors/RangeSensorObject.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

class WheelMotor;
class LineDetectorObject;

class Sumobot : public SceneObject
{
public:
    enum class WheelMotorIndex { Left, Right, FrontLeft, FrontRight, BackLeft, BackRight };
    enum class RangeSensorIndex { Left, Right, FrontLeft, Front, FrontRight };
    enum class LineDetectorIndex { Left, Right, FrontLeft, FrontRight, BackLeft, BackRight };
    struct Specification {
        float bodyWidth;
        float bodyLength;
        float bodyMass;
        float wheelWidth;
        float wheelDiameter;
        float wheelMass;
        SumobotBody::Shape bodyShape;
        SumobotBody::TextureType bodyTexture;
        WheelMotor::TextureType wheelTexture;
        std::vector<std::tuple<WheelMotorIndex, glm::vec2>> wheelMotorTuples;
        std::vector<std::tuple<RangeSensorIndex, glm::vec2, RangeSensorObject::Specification>> rangeSensorTuples;
        std::vector<std::tuple<LineDetectorIndex, glm::vec2>> lineDetectorTuples;
    };
    enum class Blueprint { Nsumo, TwoWheelRound, TwoWheelRectangle };
    static const Specification &getBlueprintSpec(Blueprint blueprint);
    static void sanityCheckSpec(const Specification &spec);

    Sumobot(Scene *scene, const Specification &spec,
            const glm::vec2 &startPosition, const float startRotation);
    ~Sumobot();
    float *getVoltageLine(WheelMotorIndex wheelMotorIndex) const;

private:
    void createBody(const Specification &spec, const glm::vec2 &startPosition, const float startRotation);
    void createWheelMotors(const Specification &spec);
    void createSensors(const Specification &spec);

    std::unique_ptr<SumobotBody> m_sumobotBody;
    std::unordered_map<WheelMotorIndex, std::unique_ptr<WheelMotor>> m_wheelMotors;
    std::unordered_map<RangeSensorIndex, std::unique_ptr<RangeSensorObject>> m_rangeSensors;
    std::unordered_map<LineDetectorIndex, std::unique_ptr<LineDetectorObject>> m_lineDetectors;
};

#endif /* SUMOBOT_H_ */




