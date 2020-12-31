#ifndef CIRCLE_OBJECT_
#define CIRCLE_OBJECT_

#include "SceneObject.h"
#include "components/Body2D.h"
#include <glm/glm.hpp>

/**
 * Simulates a simple circular object with physics (optional). It's needed because
 * a scene can't directly update components (CircleComponent).
 */
class CircleObject : public SceneObject
{
public:
    CircleObject(Scene *scene, const glm::vec4 &color, const Body2D::Specification *spec,
                 const glm::vec2 &position, float radius);
    CircleObject(Scene *scene, const glm::vec4 &fillColor, const glm::vec4 &borderColor,
                 const glm::vec2 &position, float innerRadius, float outerRadius);
    ~CircleObject();
};

#endif /* CIRCLE_OBJECT_H_ */
