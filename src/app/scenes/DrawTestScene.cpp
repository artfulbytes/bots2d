#include "DrawTestScene.h"
#include "shapes/QuadObject.h"
#include "shapes/CircleObject.h"
#include "shapes/LineObject.h"

#include <glm/glm.hpp>

DrawTestScene::DrawTestScene()
{
    m_quadObject = std::make_unique<QuadObject>(this, glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, nullptr,
                                                glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 2.0f, 2.0f }, 2.0f);
    m_circleObject = std::make_unique<CircleObject>(this, glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f },
                                                    glm::vec2{ 0.0f, 3.0f }, 1.0f);
    m_lineObject = std::make_unique<LineObject>(this, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f }, glm::vec2{ -1.0f, -3.0f },
                                                glm::vec2{ 1.0f, -1.5f }, 0.05f);
}
