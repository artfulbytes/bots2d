#include "playgrounds/Dohyo.h"
#include "PhysicsWorld.h"
#include "components/Transforms.h"
#include "components/Body2D.h"
#include "components/HollowCircleComponent.h"
#include "components/QuadComponent.h"
#include "shapes/QuadObject.h"

Dohyo::Dohyo(Scene *scene, const Specification &spec, const glm::vec2 &position) :
    SceneObject(scene)
{
    assert(m_physicsWorld->getGravityType() == PhysicsWorld::Gravity::TopView);
    m_transformComponent = std::make_unique<HollowCircleTransform>(position, spec.innerRadius, spec.outerRadius);
    const auto transform = static_cast<HollowCircleTransform *>(m_transformComponent.get());
    m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, Body2D::Specification{ false, false, 0.0f });
    static_cast<Body2D *>(m_physicsComponent.get())->setUserData(&m_userData);

    switch (spec.textureType) {
    case Dohyo::TextureType::Scratched:
    {
        m_quadObject = std::make_unique<QuadObject>(scene, "dohyo_scratched.png", nullptr, nullptr,
                                                    position,
                                                    glm::vec2{ 2.0f * spec.outerRadius, 2.0f * spec.outerRadius },
                                                    0.0f);
        break;
    }
    case Dohyo::TextureType::None:
    {
        m_renderableComponent = std::make_unique<HollowCircleComponent>(transform, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f },
                                                                        glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
        break;
    }
    }
}

Dohyo::~Dohyo()
{
}

void Dohyo::onFixedUpdate(double stepTime)
{
    (void)stepTime;
}
