#ifndef RANGE_SENSOR_OBJECT_H_
#define RANGE_SENSOR_OBJECT_H_

#include "SceneObject.h"
#include "PhysicsWorld.h"

class RangeSensor;
class Body2D;

class RangeSensorObject : public SceneObject
{
public:
    struct Specification {
        const float relativeAngle = 0.0f;
        const float minDistance = 0.0f;
        const float maxDistance = 0.0f;
    };
    RangeSensorObject(Scene *scene, const Specification &spec, bool debugShow,
                      const glm::vec2 startPosition = { 0.0f, 0.0f });
    ~RangeSensorObject();
    Body2D *getBody() const;
    void onFixedUpdate(double stepTime) override;
    float *getVoltageLine() const;

private:
    RangeSensor *m_rangeSensor = nullptr;
};

#endif /* RANGE_SENSOR_OBJECT_H_ */
