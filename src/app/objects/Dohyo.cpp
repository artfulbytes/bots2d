#include "Dohyo.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include "HollowCircleComponent.h"
#include "Body2D.h"

Dohyo::Dohyo(Scene *scene, const PhysicsWorld &world, const Specification &unscaledSpec, const glm::vec2 &unscaledStartPos) :
    SceneObject(scene),
    m_scaledSpec(scaleSpec(unscaledSpec))
{
    assert(world.getGravityType() == PhysicsWorld::Gravity::TopView);
    m_transformComponent = std::make_unique<HollowCircleTransform>(unscaledStartPos, unscaledSpec.innerRadius,
                                                                   unscaledSpec.outerRadius);
    const auto transform = static_cast<HollowCircleTransform *>(m_transformComponent.get());
    m_physicsComponent = std::make_unique<Body2D>(world, transform, Body2D::BodySpec{ false, false, 0.0f });
    static_cast<Body2D *>(m_physicsComponent.get())->setUserData(&m_userData);

    switch (unscaledSpec.textureType) {
    case Dohyo::TextureType::Scratched:
    {
        m_quadObject = std::make_unique<QuadObject>(scene, "../resources/textures/dohyo_scratched.png", nullptr, nullptr,
                                                    PhysicsWorld::scalePosition(unscaledStartPos),
                                                    glm::vec2{ 2.0f * m_scaledSpec.outerRadius, 2.0f * m_scaledSpec.outerRadius },
                                                    0.0f);
        break;
    }
    case Dohyo::TextureType::None:
    {
        m_renderableComponent = std::make_unique<HollowCircleComponent>(transform, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
        break;
    }
    }
}

Dohyo::Specification Dohyo::scaleSpec(const Specification &unscaledSpec)
{
    const Specification scaledSpec = {
        .innerRadius = PhysicsWorld::scaleLength(unscaledSpec.innerRadius),
        .outerRadius = PhysicsWorld::scaleLength(unscaledSpec.outerRadius)
    };
    return scaledSpec;
}

void Dohyo::onFixedUpdate(double stepTime)
{
}

Dohyo::~Dohyo()
{
}
