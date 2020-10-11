#ifndef RANGE_SENSOR_H_
#define RANGE_SENSOR_H_

#include <box2d/box2d.h>

class RangeSensor
{
public:
    RangeSensor(b2World* world, b2Body* body, b2Vec2 position, float angle, float minDistance, float maxDistance);
    float getDistance(void);
    void update(void);

private:
    b2World* m_world;
    b2Body* m_body;
    float m_angle = 0.0f;
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_minDistance = 0.0f;
    float m_maxDistance = 0.0f;
    b2Vec2 m_rayPoints[2];

    /* Relative position to the body that owns the sensor */
    b2Vec2 m_position;

    b2Vec2 m_rayStartPosition;
    b2Vec2 m_rayEndPosition;
};

#endif
