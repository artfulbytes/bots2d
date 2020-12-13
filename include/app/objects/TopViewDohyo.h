#ifndef TOP_VIEW_DOHYO_H_
#define TOP_VIEW_DOHYO_H_

#include "AppObject.h"
#include "PhysicsWorld.h"
#include "Body2DUserData.h"

class Body2D;

class TopViewDohyo : public AppObject
{
public:
    enum class TextureType { Scratched, None };
    struct Specification {
        const float innerRadius;
        const float outerRadius;
        const TextureType textureType = TextureType::None;
    };
    TopViewDohyo(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2<float> &unscaledStartPos);
    ~TopViewDohyo();
    void onFixedUpdate(double stepTime) override;

private:
    Specification scaleSpec(const Specification &unscaledSpec);
    Body2D *m_body2D = nullptr;
    Body2DUserData m_userData = { 0, 0, BodyId::DohyoBorder };
    Specification m_scaledSpec;
};

#endif /* TOP_VIEW_DOHYO_H_ */
