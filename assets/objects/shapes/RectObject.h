#ifndef RECT_OBJECT_H_
#define RECT_OBJECT_H_

#include "SceneObject.h"
#include "components/Body2D.h"
#include "SpriteAnimation.h"

#include <glm/glm.hpp>
#include <string>

/**
 * A renderable rectangle with physics (optional). It's needed because a scene can't directly update
 * components (RectComponent and Body2D).
 */
class RectObject : public SceneObject
{
public:
    RectObject(Scene *scene, const glm::vec4 &color,
               const Body2D::Specification *spec,
               const glm::vec2 &position, const glm::vec2 &size, float rotation);
    RectObject(Scene *scene, const std::string& textureFilepath, const SpriteAnimation::Params *animationParams,
               const Body2D::Specification *spec,
               const glm::vec2 &position, const glm::vec2 &size, float rotation);
    Body2D *getBody() const;

private:
    std::unique_ptr<SpriteAnimation> m_animation;
};

#endif /* RECT_OBJECT_H_ */
