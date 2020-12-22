#include "PhysicsTestScene.h"
#include "shapes/QuadObject.h"
#include "shapes/CircleObject.h"

#include <glm/glm.hpp>

PhysicsTestScene::PhysicsTestScene() :
    Scene("Test physics with 5x5cm@1kg box and 5cm@1kg ball", PhysicsWorld::Gravity::SideView)
{
    const glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    const Body2D::Specification groundBodySpec = { false, true, 1.0f };
    m_ground = std::make_unique<QuadObject>(this, color, &groundBodySpec,
                                            glm::vec2{ 0.0f, -0.2f }, glm::vec2{ 0.4f, 0.025f}, 0.0f);

    const Body2D::Specification fallingBoxBodySpec = { true, true, 1.0f };
    m_fallingBox = std::make_unique<QuadObject>(this, color, &fallingBoxBodySpec,
                                                glm::vec2{ 0.0f, 0.5f }, glm::vec2{ 0.05f, 0.05f }, 0.2f);
    const Body2D::Specification fallingBallBodySpec = { true, true, 1.0f };
    m_fallingBall = std::make_unique<CircleObject>(this, color, &fallingBallBodySpec, glm::vec2{ 0.04f, 0.7f }, 0.025f);
}
