#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_

#include "Component.h"

#include <glm/glm.hpp>

/* Transform act as the link between physics and rendering */
struct TransformComponent : Component
{
    /* Add virtual destructor for polymorphism */
    virtual ~TransformComponent() {};
};

struct LineTransform : TransformComponent
{
    LineTransform() {}
    LineTransform(const glm::vec2 &start, const glm::vec2 &end, float width) :
        start(start), end(end), width(width) {}
    glm::vec2 start;
    glm::vec2 end;
    float width;
};

struct QuadTransform : TransformComponent
{
    QuadTransform(const glm::vec2 &position, const glm::vec2 &size, float rotation = 0.0f) :
        position(position), size(size), rotation(rotation) {}
    ~QuadTransform() {}
    glm::vec2 position;
    glm::vec2 size;
    float rotation;
};

struct CircleTransform : TransformComponent
{
    CircleTransform() {}
    CircleTransform(const glm::vec2 &position, float radius) :
        position(position), radius(radius) {}
    ~CircleTransform() {}
    glm::vec2 position;
    float radius;
};

struct HollowCircleTransform : TransformComponent
{
    HollowCircleTransform(const glm::vec2 &position, float innerRadius, float outerRadius) :
        position(position), innerRadius(innerRadius), outerRadius(outerRadius) {}
    glm::vec2 position;
    float innerRadius;
    float outerRadius;
};

#endif /* TRANSFORMS_H_ */
