#ifndef LINE_DETECTOR_OBJECT_H_
#define LINE_DETECTOR_OBJECT_H_

#include "SceneObject.h"
#include "PhysicsWorld.h"

class LineDetector;
class Body2D;

/**
 * Line detector with debug draw to notice detection graphically.
 */
class LineDetectorObject : public SceneObject
{
public:
    struct Specification {
        const float updateRateSeconds = 0.0f;
    };
    LineDetectorObject(Scene *scene, const Specification &spec, bool debugDraw,
                       const glm::vec2 &startPosition = { 0.0f, 0.0f });
    ~LineDetectorObject();
    Body2D *getBody() const;
    void onFixedUpdate() override;
    float *getVoltageLine() const;
    void setDebugDraw(bool enable);

private:
    LineDetector *m_lineDetector = nullptr;
    bool m_debugDrawEnabled = false;
};

#endif /* LINE_DETECTOR_OBJECT_H_ */
