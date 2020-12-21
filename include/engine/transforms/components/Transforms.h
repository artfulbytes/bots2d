#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_

#include "Component.h"

#include <glm/glm.hpp>

/* Transform act as the link between physics and rendering */
class TransformComponent : public Component
{
public:
    /* Add virtual destructor for polymorphism */
    virtual ~TransformComponent() {};
};

class LineTransform : public TransformComponent
{
public:
    LineTransform() {}
    LineTransform(const glm::vec2 &start, const glm::vec2 &end, float width) :
        start(start), end(end), width(width) {}
    glm::vec2 start;
    glm::vec2 end;
    float width;
};

class QuadTransform : public TransformComponent
{
public:
    QuadTransform(const glm::vec2 &position, const glm::vec2 &size, float rotation = 0.0f) :
        position(position), size(size), rotation(rotation) {}
    ~QuadTransform() {}
    glm::vec2 position;
    glm::vec2 size;
    float rotation;
};

class CircleTransform : public TransformComponent
{
public:
    CircleTransform() {}
    CircleTransform(const glm::vec2 &position, float radius) :
        position(position), radius(radius) {}
    ~CircleTransform() {}
    glm::vec2 position;
    float radius;
};

class HollowCircleTransform : public TransformComponent
{
public:
    HollowCircleTransform(const glm::vec2 &position, float innerRadius, float outerRadius) :
        position(position), innerRadius(innerRadius), outerRadius(outerRadius) {}
    ~HollowCircleTransform() {}
    glm::vec2 position;
    float innerRadius;
    float outerRadius;
};

#endif /* TRANSFORMS_H_ */
