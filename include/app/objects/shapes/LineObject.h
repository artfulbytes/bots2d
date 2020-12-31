#ifndef LINE_OBJECT_H_
#define LINE_OBJECT_H_

#include "SceneObject.h"
#include <glm/glm.hpp>

/**
 * Draws a line in a scene. It's needed because a scene can't directly
 * update components (LineComponent).
 */
class LineObject : public SceneObject
{
public:
    LineObject(Scene *scene, const glm::vec4 &color, const glm::vec2 &start, const glm::vec2 &end, float width);
};

#endif /* LINE_OBJECT_H_ */
