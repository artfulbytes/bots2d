#ifndef DOHYO_H_
#define DOHYO_H_

#include <box2d/box2d.h>
#include "Constants.h"

class Dohyo
{
public:
    Dohyo(b2World* world, float x, float y, float innerRadius, float outerRadius);
    ~Dohyo();

    b2Body* getBody() { return m_body; }
private:
    constexpr static int totalVertexCount = 180;
    constexpr static float anglePerVertex = 2 * constants::pi / static_cast<float>(totalVertexCount);
    constexpr static int trapezoidVertexCount = 4;

    b2Body* m_body = nullptr;
    float m_x, m_y;
    float m_innerRadius, m_outerRadius;

    void createBorder(void);
};

#endif /* DOHYO_H_ */
