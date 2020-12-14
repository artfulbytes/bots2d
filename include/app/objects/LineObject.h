#ifndef LINE_OBJECT_H_
#define LINE_OBJECT_H_

#include "SceneObject.h"
#include <glm/glm.hpp>

class LineObject : public SceneObject
{
public:
    LineObject(Scene *scene, const glm::vec4 &color, const glm::vec2 &start, const glm::vec2 &end, float width);
};

#endif /* LINE_OBJECT_H_ */
