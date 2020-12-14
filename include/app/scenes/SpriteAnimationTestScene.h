#ifndef SPRITE_ANIMATION_TEST_SCENE_H_
#define SPRITE_ANIMATION_TEST_SCENE_H_

#include "Scene.h"

class QuadObject;

class SpriteAnimationTestScene : public Scene
{
public:
    SpriteAnimationTestScene();
private:
    std::unique_ptr<QuadObject> m_quadObject;
};

#endif /* SPRITE_ANIMATION_TEST_SCENE_H_ */
