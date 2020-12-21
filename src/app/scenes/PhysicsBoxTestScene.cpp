#include "PhysicsBoxTestScene.h"
#include "shapes/QuadObject.h"

#include <glm/glm.hpp>

PhysicsBoxTestScene::PhysicsBoxTestScene() :
    Scene(PhysicsWorld::Gravity::SideView)
{
    const glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    const Body2D::BodySpec groundBodySpec =
    {
        dynamic : false,
        collision : true,
        massUnscaled : 1.0f
    };
    m_ground = std::make_unique<QuadObject>(this, color, &groundBodySpec,
                                            glm::vec2{ 0.0f, -0.2f }, glm::vec2{ 0.4f, 0.025f}, 0.0f);

    const Body2D::BodySpec fallingBoxBodySpec =
    {
        dynamic : true,
        collision : true,
        massUnscaled : 1.0f
    };
    m_fallingBox = std::make_unique<QuadObject>(this, color, &fallingBoxBodySpec,
                                                glm::vec2{ 0.0f, 0.5f }, glm::vec2{ 0.05f, 0.05f }, 0.2f);
}
