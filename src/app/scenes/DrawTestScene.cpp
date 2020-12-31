#include "DrawTestScene.h"
#include "shapes/RectObject.h"
#include "shapes/QuadObject.h"
#include "shapes/CircleObject.h"
#include "shapes/LineObject.h"
#include "QuadCoords.h"

#include <glm/glm.hpp>

#include "Renderer.h"

DrawTestScene::DrawTestScene() :
    Scene("Test draw basic shapes")
{
    m_rectObject = std::make_unique<RectObject>(this, glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, nullptr,
                                                glm::vec2{ 0.0f, 0.7f }, glm::vec2{ 0.25f, 0.25f }, 0.0f);
    const QuadCoords quadCoords
    {
        { -0.15f,  0.20f },
        {  0.10f,  0.20f },
        {  0.15f,  0.45f },
        { -0.10f,  0.45f },
    };
    m_quadObject = std::make_unique<QuadObject>(this, quadCoords, glm::vec4{1.0f, 1.0f, 0.0f, 1.0f}, nullptr, false);
    m_circleObject = std::make_unique<CircleObject>(this, glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f }, nullptr,
                                                    glm::vec2{ 0.0f, 0.0f }, 0.125f);
    m_lineObject = std::make_unique<LineObject>(this, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f }, glm::vec2{ -0.125f, -0.5f },
                                                glm::vec2{ 0.125f, -0.25f }, 0.005f);
}
