#ifndef LINE_DETECTOR_OBJECT_H_
#define LINE_DETECTOR_OBJECT_H_

#include "SceneObject.h"
#include "PhysicsWorld.h"

class LineDetector;
class Body2D;

class LineDetectorObject : public SceneObject
{
public:
    LineDetectorObject(Scene *scene, const PhysicsWorld &world, Body2D &parentBody,
                       bool show, const glm::vec2 &unscaledRelativePosition);
    ~LineDetectorObject();
    void onFixedUpdate(double stepTime) override;
    float *getVoltageLine() const;

private:
    LineDetector *m_lineDetector = nullptr;
};

#endif /* LINE_DETECTOR_OBJECT_H_ */
