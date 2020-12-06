#ifndef LINE_DETECTOR_OBJECT_H_
#define LINE_DETECTOR_OBJECT_H_

#include "AppObject.h"
#include "PhysicsWorld.h"

class LineDetector;
class Body2D;

class LineDetectorObject : public AppObject
{
public:
    LineDetectorObject(AppScene &appScene, const PhysicsWorld &world, Body2D &parentBody,
                       bool show, const Vec2<float> &unscaledRelativePosition);
    ~LineDetectorObject();
    void onFixedUpdate(double stepTime) override;
    float *getVoltageLine() const;

private:
    LineDetector *m_lineDetector = nullptr;
};

#endif /* LINE_DETECTOR_OBJECT_H_ */
