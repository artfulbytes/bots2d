#ifndef RANGE_SENSOR_H_
#define RANGE_SENSOR_H_

#include "PhysicsComponent.h"

class Body2D;
class LineTransform;

class RangeSensor : public PhysicsComponent
{
public:
    RangeSensor(const PhysicsWorld &world, LineTransform *transform, const Body2D &body,
                const Vec2<float> &position, float angle, float minDistance, float maxDistance);
    ~RangeSensor();
    void onFixedUpdate(double stepTime) override;
    float getDistance() const;
    float *getVoltageLine();

private:
    void updateVoltage();
    void updateDetectedDistance(const Vec2<float> &start, const Vec2<float> &end);

    const float m_minDistance = 0.0f;
    const float m_maxDistance = 0.0f;
    const float m_relativeAngle = 0.0f;
    const Vec2<float> m_relativePosition;
    const Body2D *m_parentBody = nullptr;

    LineTransform * const m_lineTransform = nullptr;
    float m_distanceVoltage = 0.0f;
    float m_detectedDistance = 0.0f;
};

#endif /* RANGE_SENSOR_H_ */
