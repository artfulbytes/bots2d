#include "DrawTestScene.h"
#include "shapes/QuadObject.h"
#include "shapes/CircleObject.h"
#include "shapes/LineObject.h"

#include <glm/glm.hpp>

DrawTestScene::DrawTestScene() :
    Scene("Test draw basic shapes")
{
    m_quadObject = std::make_unique<QuadObject>(this, glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, nullptr,
                                                glm::vec2{ 0.0f, 0.4f }, glm::vec2{ 0.25f, 0.25f }, 2.0f);
    m_circleObject = std::make_unique<CircleObject>(this, glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f }, nullptr,
                                                    glm::vec2{ 0.0f, 0.0f }, 0.125f);
    m_lineObject = std::make_unique<LineObject>(this, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f }, glm::vec2{ -0.125f, -0.5f },
                                                glm::vec2{ 0.125f, -0.25f }, 0.005f);
}
