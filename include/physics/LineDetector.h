#ifndef LINE_DETECTOR_H_
#define LINE_DETECTOR_H_

#include <box2d/box2d.h>

class LineDetector
{
public:
    LineDetector(b2World* world, b2Body* ownerBody, b2Vec2 position);
    bool detected(void);

private:
    b2World* m_world = nullptr;
    b2Body* m_body = nullptr;
    b2Body* m_ownerBody = nullptr;
    b2Joint* m_joint = nullptr;
    b2Vec2 m_position;
};

#endif /* LINE_DETECTOR_H */
