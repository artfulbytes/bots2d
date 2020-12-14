#ifndef CIRCLE_OBJECT_
#define CIRCLE_OBJECT_

#include "SceneObject.h"
#include <glm/glm.hpp>

class CircleObject : public SceneObject
{
public:
    CircleObject(Scene *scene, const glm::vec4 &color, const glm::vec2 &position, float radius);
    CircleObject(Scene *scene, const glm::vec4 &fillColor, const glm::vec4 &borderColor,
                 const glm::vec2 &position, float innerRadius, float outerRadius);
    ~CircleObject();
};

#endif /* CIRCLE_OBJECT_H_ */
