#include "TopViewDohyo.h"
#include "Transforms.h"
#include "CircleComponent.h"
#include "Body2D.h"

TopViewDohyo::TopViewDohyo(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2 &unscaledStartPos) :
    AppObject(appScene),
    m_scaledSpec(scaleSpec(unscaledSpec))
{
    assert(world.getGravityType() == PhysicsWorld::Gravity::TopView);
    HollowCircleTransform *transformBody = new HollowCircleTransform();
    transformBody->position.x = unscaledStartPos.x;
    transformBody->position.y = unscaledStartPos.y;
    transformBody->position.z = 0.0f;
    transformBody->innerRadius = unscaledSpec.innerRadius;
    transformBody->outerRadius = unscaledSpec.outerRadius;
    m_body2D = new Body2D(world, *transformBody, false, 0.0f);

    /* For simplicity create two overlapping circles */
    /* Big circle */
    CircleTransform *transformOuterBorder = new CircleTransform();
    transformOuterBorder->position.x = PhysicsWorld::scalePosition(unscaledStartPos.x);
    transformOuterBorder->position.y = PhysicsWorld::scalePosition(unscaledStartPos.y);
    transformOuterBorder->radius = m_scaledSpec.outerRadius;
    glm::vec4 colorOuter(1.0f, 1.0f, 1.0f, 1.0f);
    CircleComponent *circleComponent = new CircleComponent(colorOuter);
    appScene.getScene()->createObject(transformOuterBorder,
                                      new CircleComponent(colorOuter),
                                      nullptr, nullptr);
    /* Smaller inner circle */
    CircleTransform *transformInnerBorder = new CircleTransform();
    transformInnerBorder->position.x = PhysicsWorld::scalePosition(unscaledStartPos.x);
    transformInnerBorder->position.y = PhysicsWorld::scalePosition(unscaledStartPos.y);
    transformInnerBorder->radius = m_scaledSpec.innerRadius;
    glm::vec4 colorInner(0.0f, 0.0f, 0.0f, 1.0f);
    appScene.getScene()->createObject(transformInnerBorder,
                                      new CircleComponent(colorInner),
                                      nullptr, nullptr);

}

TopViewDohyo::Specification TopViewDohyo::scaleSpec(const Specification &unscaledSpec)
{
    const Specification scaledSpec = {
        .innerRadius = PhysicsWorld::scaleLength(unscaledSpec.innerRadius),
        .outerRadius = PhysicsWorld::scaleLength(unscaledSpec.outerRadius)
    };
    return scaledSpec;
}

void TopViewDohyo::onFixedUpdate(double stepTime)
{
}

TopViewDohyo::~TopViewDohyo()
{
}
