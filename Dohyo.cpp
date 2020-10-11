#include "Dohyo.h"
#include "Constants.h"
#include "UserData.h"

using namespace constants;

Dohyo::Dohyo(b2World* world, float x, float y, float innerRadius, float outerRadius) :
    m_x(x),
    m_y(y),
    m_innerRadius(innerRadius * lengthScaleFactor),
    m_outerRadius(outerRadius * lengthScaleFactor)
{
    assert(outerRadius > innerRadius);

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x * lengthScaleFactor, y * lengthScaleFactor);
    m_body = world->CreateBody(&bodyDef);

    UserData* userData = new UserData();
    userData->bodyType = BodyType::DohyoBorder;
    m_body->SetUserData(userData);

    createBorder();
}

void Dohyo::createBorder()
{
    b2PolygonShape shape;
    b2Vec2 trapezoidVertices[trapezoidVertexCount];
    float angle=0;
    for (int cornerIdx=0; cornerIdx < totalVertexCount; cornerIdx++) {
        angle = cornerIdx * anglePerVertex;
        trapezoidVertices[0].Set(m_innerRadius * cosf(angle), m_innerRadius * sinf(angle));
        trapezoidVertices[1].Set(m_outerRadius * cosf(angle), m_outerRadius * sinf(angle));
        angle += anglePerVertex;
        trapezoidVertices[2].Set(m_outerRadius * cosf(angle), m_outerRadius * sinf(angle));
        trapezoidVertices[3].Set(m_innerRadius * cosf(angle), m_innerRadius * sinf(angle));
        shape.Set(trapezoidVertices, trapezoidVertexCount);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.isSensor = true;
        m_body->CreateFixture(&fixtureDef);
    }
}
