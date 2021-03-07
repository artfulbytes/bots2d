#ifndef PHYSICS_BOT_TEST_SCENE_H_
#define PHYSICS_BOT_TEST_SCENE_H_

#include "Scene.h"

class KeyboardController;
class PhysicsBot;

class PhysicsBotTestScene : public Scene
{
public:
    PhysicsBotTestScene();
    ~PhysicsBotTestScene();

private:
    std::unique_ptr<KeyboardController> m_keyboardController;
    std::unique_ptr<PhysicsBot> m_physicsBot;
};

#endif /* PHYSICS_BOT_TEST_SCENE_H_ */
