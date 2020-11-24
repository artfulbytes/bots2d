#ifndef LINE_DETECTOR_H_
#define LINE_DETECTOR_H_

#include "PhysicsComponent.h"
#include "Body2DUserData.h"

class Body2D;
class b2Body;
class CircleTransform;

class LineDetector : public PhysicsComponent
{
public:
    LineDetector(const PhysicsWorld &world, CircleTransform *transform, Body2D &parentBody, const Vec2 &relativePosition);
    ~LineDetector();
    void onFixedUpdate(double stepTime) override;
    float *getVoltageLine();

private:
    b2Body *m_body = nullptr;
    Body2DUserData m_userData = { 0, 0, BodyId::LineDetector };
    CircleTransform * const m_transform = nullptr;
    float m_detectVoltage = 0.0f;
};

#endif /* LINE_DETECTOR_H_ */
