#ifndef TOP_VIEW_DOHYO_H_
#define TOP_VIEW_DOHYO_H_

#include "SceneObject.h"
#include "PhysicsWorld.h"
#include "Body2DUserData.h"
#include "QuadObject.h"

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
    Dohyo(Scene *scene, const PhysicsWorld &world, const Specification &unscaledSpec, const glm::vec2 &unscaledStartPos);
    ~Dohyo();
    void onFixedUpdate(double stepTime) override;

private:
    Specification scaleSpec(const Specification &unscaledSpec);
    Body2DUserData m_userData = { 0, 0, BodyId::DohyoBorder };
    Specification m_scaledSpec;
    std::unique_ptr<QuadObject> m_quadObject;
};

#endif /* TOP_VIEW_DOHYO_H_ */
