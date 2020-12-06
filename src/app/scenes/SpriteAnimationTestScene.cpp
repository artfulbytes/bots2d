#include "SpriteAnimationTestScene.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include "SpriteAnimation.h"
#include <glm/glm.hpp>

SpriteAnimationTestScene::SpriteAnimationTestScene()
{
    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    QuadTransform *transform = new QuadTransform();
    transform->position.x = 0.0f;
    transform->position.y = 0.0f;
    transform->position.z = 0.0f;
    transform->size.x = 7.5f;
    transform->size.y = 7.5f;
    transform->rotation = 0.0f;
    SpriteAnimation *spriteAnimation = new SpriteAnimation(Vec2<unsigned int>{1, 2}, 2, 30);
    QuadComponent *quadComponent = new QuadComponent("../resources/textures/AnimationTestSpriteSheet.png", *spriteAnimation);
    m_scene->createObject(transform, quadComponent, nullptr, nullptr);
}
