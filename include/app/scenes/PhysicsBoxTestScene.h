#ifndef PHYSICS_BOX_TEST_SCENE_H_
#define PHYSICS_BOX_TEST_SCENE_H_

#include "Scene.h"

class QuadObject;

class PhysicsBoxTestScene : public Scene
{
public:
    PhysicsBoxTestScene();

private:
    std::unique_ptr<QuadObject> m_ground;
    std::unique_ptr<QuadObject> m_fallingBox;
};

#endif /* PHYSICS_BOX_TEST_SCENE_H_ */
