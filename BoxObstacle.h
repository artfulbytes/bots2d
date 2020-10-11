#ifndef BOX_OBSTACLE_H_
#define BOX_OBSTACLE_H_

#include "box2d/box2d.h"

class BoxObstacle
{
public:
    BoxObstacle(b2World* world, float x, float y, float width, float mass);

private:
    b2Body* m_body = nullptr;
    b2Body* m_frictionBody = nullptr;
    b2Joint* m_frictionJoint = nullptr;

    constexpr static float frictionCoefficient = 0.1f;
};
#endif /* BOX_OBSTACLE_H_ */
