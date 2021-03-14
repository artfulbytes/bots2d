#include "shapes/QuadObject.h"
#include "components/QuadComponent.h"
#include "components/Transforms.h"

QuadObject::QuadObject(Scene *scene, const QuadCoords &quadCoords, const glm::vec4 &color,
                       const Body2D::Specification *spec, bool detectable) :
    SceneObject(scene)
{
    m_transformComponent = std::make_unique<QuadTransform>(quadCoords);
    auto transform = static_cast<QuadTransform *>(m_transformComponent.get());
    if (spec != nullptr) {
        m_physicsComponent = std::make_unique<Body2D>(*m_physicsWorld, transform, *spec);
        m_body2D = static_cast<Body2D *>(m_physicsComponent.get());
        if (detectable) {
            m_body2D->setUserData(&m_userData);
        }
    }

    m_renderableComponent = std::make_unique<QuadComponent>(transform, color);
}
