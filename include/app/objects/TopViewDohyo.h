#ifndef TOP_VIEW_DOHYO_H_
#define TOP_VIEW_DOHYO_H_

#include "AppObject.h"
#include "PhysicsWorld.h"

class Body2D;

class TopViewDohyo : public AppObject
{
public:
    struct Specification {
        const float innerRadius;
        const float outerRadius;
    };
    TopViewDohyo(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2 &unscaledStartPos);
    ~TopViewDohyo();
    void onFixedUpdate(double stepTime) override;

private:
    Specification scaleSpec(const Specification &unscaledSpec);
    Body2D *m_body2D = nullptr;
    Specification m_scaledSpec;
};

#endif /* TOP_VIEW_DOHYO_H_ */
