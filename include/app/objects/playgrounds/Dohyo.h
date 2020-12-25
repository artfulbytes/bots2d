#ifndef DOHYO_H_
#define DOHYO_H_

#include "SceneObject.h"
#include "Body2DUserData.h"
#include <glm/glm.hpp>

class PhysicsWorld;
class QuadObject;
class Body2D;

class Dohyo : public SceneObject
{
public:
    enum class TextureType { Scratched, None };
    struct Specification {
        const float innerRadius;
        const float outerRadius;
        const TextureType textureType = TextureType::None;
    };
    Dohyo(Scene *scene, const Specification &spec, const glm::vec2 &position);
    ~Dohyo();
    void onFixedUpdate(double stepTime) override;

private:
    Body2DUserData m_userData = { 0, 0, BodyId::DohyoBorder };
    std::unique_ptr<QuadObject> m_quadObject;
};

#endif /* DOHYO_H_ */
