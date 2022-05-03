#ifndef LINE_DETECTOR_H_
#define LINE_DETECTOR_H_

#include "PhysicsComponent.h"
#include "Body2DUserData.h"

class Body2D;
class b2Body;
class CircleTransform;

/**
 * A perfect line detector sensor, which detects physics bodies with BodyId set to
 * "Detectable" (see Body2DUserData).
 */
class LineDetector : public PhysicsComponent
{
public:
    LineDetector(const PhysicsWorld &world, CircleTransform *transform, const glm::vec2 &startPosition,
                 float updateRateSeconds = 0.0f);
    ~LineDetector();
    void onFixedUpdate(float stepTime) override;
    Body2D *getBody() const;
    /** Retrieve a pointer to the voltage line where the value is > 0 when detected and 0 when not detected. */
    float *getVoltageLine();

private:
    std::unique_ptr<Body2D> m_body2D;
    Body2DUserData m_userData = { 0, 0, BodyId::LineDetector };
    CircleTransform * const m_transform = nullptr;
    float m_detectVoltage = 0.0f;
    float m_updateRateSeconds = 0.0f;
    float m_timeSinceLastUpdate = 0.0f;
};

#endif /* LINE_DETECTOR_H_ */
