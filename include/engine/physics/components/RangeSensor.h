#ifndef RANGE_SENSOR_H_
#define RANGE_SENSOR_H_

#include "PhysicsComponent.h"

class Body2D;
class LineTransform;

class RangeSensor : public PhysicsComponent
{
public:
    RangeSensor(const PhysicsWorld &world, LineTransform *transform,
                const glm::vec2 &startPosition, float angle, float minDistance, float maxDistance);
    ~RangeSensor();
    void onFixedUpdate(double stepTime) override;
    float getDistance() const;
    Body2D *getBody() const;
    float *getVoltageLine();

private:
    void updateVoltage();
    void updateDetectedDistance(const glm::vec2 &start, const glm::vec2 &end);

    const float m_minDistance = 0.0f;
    const float m_maxDistance = 0.0f;
    const float m_relativeAngle = 0.0f;
    std::unique_ptr<Body2D> m_body2D;

    LineTransform * const m_lineTransform = nullptr;
    float m_distanceVoltage = 0.0f;
    float m_detectedDistance = 0.0f;
};

#endif /* RANGE_SENSOR_H_ */
