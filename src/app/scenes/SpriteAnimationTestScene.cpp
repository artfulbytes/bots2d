#include "SpriteAnimationTestScene.h"
#include "SpriteAnimation.h"
#include "QuadObject.h"
#include <glm/glm.hpp>

SpriteAnimationTestScene::SpriteAnimationTestScene()
{
    const SpriteAnimation::Params animationParams =
    {
        .spriteSheetWidth = 1,
        .spriteSheetHeight = 2,
        .spriteCount = 2,
        .framesBetweenUpdates = 30
    };
    m_quadObject = std::make_unique<QuadObject>(this, "../resources/textures/AnimationTestSpriteSheet.png", &animationParams,
                                                nullptr, glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 5.0f, 5.0f }, 0.0f);
}
