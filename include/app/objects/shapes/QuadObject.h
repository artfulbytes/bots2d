#ifndef QUAD_OBJECT_H_
#define QUAD_OBJECT_H_

#include "SceneObject.h"
#include "components/Body2D.h"
#include "SpriteAnimation.h"

#include <glm/glm.hpp>
#include <string>

class QuadObject : public SceneObject
{
public:
    QuadObject(Scene *scene, const glm::vec4 &color,
               const Body2D::Specification *spec,
               const glm::vec2 &position, const glm::vec2 &size, float rotation);
    QuadObject(Scene *scene, const std::string& textureFilepath, const SpriteAnimation::Params *animationParams,
               const Body2D::Specification *spec,
               const glm::vec2 &position, const glm::vec2 &size, float rotation);
private:
    std::unique_ptr<SpriteAnimation> m_animation;
};

#endif /* QUAD_OBJECT_H_ */
