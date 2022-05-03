#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_

#include "Component.h"
#include "QuadCoords.h"

#include <glm/glm.hpp>

/**
 * The base class for Transform-components. A transform holds the data
 * shared between physics and rendering and can thus be seen as the
 * link between them.
 */
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
    glm::vec2 start = {0, 0};
    glm::vec2 end = {0, 0};
    float width = 0.0f;
};

class RectTransform : public TransformComponent
{
public:
    RectTransform(const glm::vec2 &position, const glm::vec2 &size, float rotation = 0.0f) :
        position(position), size(size), rotation(rotation) {}
    ~RectTransform() {}
    glm::vec2 position;
    glm::vec2 size;
    float rotation = 0.0f;
};

class QuadTransform : public TransformComponent
{
public:
    QuadTransform(const QuadCoords &quadCoords) :
        quadCoords(quadCoords) {}
    ~QuadTransform() {}
    const QuadCoords quadCoords;
};

class CircleTransform : public TransformComponent
{
public:
    CircleTransform() {}
    CircleTransform(const glm::vec2 &position, float radius, float rotation) :
        position(position), radius(radius), rotation(rotation) {}
    ~CircleTransform() {}
    glm::vec2 position { 0, 0 };
    float radius = 0.0f;
    float rotation = 0.0f;
};

class HollowCircleTransform : public TransformComponent
{
public:
    HollowCircleTransform(const glm::vec2 &position, float innerRadius, float outerRadius) :
        position(position), innerRadius(innerRadius), outerRadius(outerRadius) {}
    ~HollowCircleTransform() {}
    glm::vec2 position;
    float innerRadius = 0.0f;
    float outerRadius = 0.0f;
};

#endif /* TRANSFORMS_H_ */
