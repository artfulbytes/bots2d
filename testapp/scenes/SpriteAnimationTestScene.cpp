#include "SpriteAnimationTestScene.h"
#include "SpriteAnimation.h"
#include "shapes/RectObject.h"

#include <glm/glm.hpp>

SpriteAnimationTestScene::SpriteAnimationTestScene() :
    Scene("Simple animation sprite sheet test")
{
    const SpriteAnimation::Params animationParams = { 1, 2, 2, 30 };
    m_rectObject = std::make_unique<RectObject>(this, "AnimationTestSpriteSheet.png", &animationParams,
                                                nullptr, glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 0.25f, 0.25f }, 0.0f);
}
