#ifndef QUAD_OBJECT_H_
#define QUAD_OBJECT_H

#include "SceneObject.h"
#include "Body2DUserData.h"
#include "components/Body2D.h"
#include <glm/glm.hpp>

struct QuadCoords;

class QuadObject : public SceneObject
{
public:
    QuadObject(Scene *scene, const QuadCoords &quadCoords, const glm::vec4 &color,
               const Body2D::Specification *spec, bool detectable);
private:
    Body2DUserData m_userData = { 0, 0, BodyId::Detectable };
    Body2D *m_body2D;
};

#endif /* QUAD_OBJECT_H_ */
