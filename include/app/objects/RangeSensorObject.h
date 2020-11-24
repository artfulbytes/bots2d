#ifndef RANGE_SENSOR_OBJECT_H_
#define RANGE_SENSOR_OBJECT_H_

#include "AppObject.h"
#include "PhysicsWorld.h"

class RangeSensor;
class Body2D;

/* TODO: Rename? */
class RangeSensorObject : public AppObject
{
public:
    struct Specification {
        const Vec2 relativePosition;
        const float relativeAngle = 0.0f;
        const float minDistance = 0.0f;
        const float maxDistance = 0.0f;
    };
    RangeSensorObject(AppScene &appScene, const PhysicsWorld &world, const Body2D &parentBody, bool show, const Specification &unscaledSpec);
    ~RangeSensorObject();
    void onFixedUpdate(double stepTime) override;
    float *getVoltageLine() const;

private:
    RangeSensor *m_rangeSensor = nullptr;
};

#endif /* RANGE_SENSOR_OBJECT_H_ */
