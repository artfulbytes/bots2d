#ifndef SPRITE_ANIMATION_TEST_SCENE_H_
#define SPRITE_ANIMATION_TEST_SCENE_H_

#include "Scene.h"

class RectObject;

class SpriteAnimationTestScene : public Scene
{
public:
    SpriteAnimationTestScene();
private:
    std::unique_ptr<RectObject> m_rectObject;
};

#endif /* SPRITE_ANIMATION_TEST_SCENE_H_ */
