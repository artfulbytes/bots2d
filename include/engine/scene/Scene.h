#ifndef SCENE_H_
#define SCENE_H_

#include "Event.h"
#include "PhysicsWorld.h"
#include <vector>
#include <memory>

class SceneObject;

class Scene
{
public:
    Scene();
    Scene(PhysicsWorld::Gravity gravity);
    virtual ~Scene();
    PhysicsWorld *getPhysicsWorld() const;
    void onFixedUpdate(double stepTime);
    void onKeyEvent(const Event::Key &keyEvent);
    void addObject(SceneObject *sceneObject);

protected:
    std::unique_ptr<PhysicsWorld> m_physicsWorld;

private:
    std::vector<SceneObject *> m_objects;
};

#endif /* SCENE_H_ */
