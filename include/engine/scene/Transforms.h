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
    glm::vec2 start;
    glm::vec2 end;
    float width;
};

struct QuadTransform : TransformComponent
{
    glm::vec3 position;
    glm::vec2 size;
    float rotation;
};

struct CircleTransform : TransformComponent
{
    glm::vec3 position;
    float radius;
};

struct HollowCircleTransform : TransformComponent
{
    glm::vec3 position;
    float innerRadius;
    float outerRadius;
};

#endif /* TRANSFORMS_H_ */
