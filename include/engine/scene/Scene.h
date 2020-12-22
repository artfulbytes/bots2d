#ifndef SCENE_H_
#define SCENE_H_

#include "Event.h"
#include "PhysicsWorld.h"
#include <vector>
#include <memory>
#include <string>

class SceneObject;

class Scene
{
public:
    Scene(std::string description);
    Scene(std::string description, PhysicsWorld::Gravity gravity);
    virtual ~Scene();
    PhysicsWorld *getPhysicsWorld() const;
    void onFixedUpdate(double stepTime);
    void onKeyEvent(const Event::Key &keyEvent);
    void addObject(SceneObject *sceneObject);
    std::string getDescription() const { return m_description; }

protected:
    std::unique_ptr<PhysicsWorld> m_physicsWorld;

private:
    std::vector<SceneObject *> m_objects;
    std::string m_description;
};

#endif /* SCENE_H_ */
