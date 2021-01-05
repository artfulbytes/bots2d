#ifndef SCENE_H_
#define SCENE_H_

#include "Event.h"
#include "PhysicsWorld.h"
#include <vector>
#include <memory>
#include <string>

class SceneObject;

/**
 * Base class for scenes. All scenes must inherit this class. A Scene provides the stage
 * for the simulation and has a PhysicsWorld and a list of SceneObject. Only one Scene
 * can be active at a time.
 */
class Scene
{
public:
    Scene(std::string description);
    Scene(std::string description, PhysicsWorld::Gravity gravity);
    virtual ~Scene();
    PhysicsWorld *getPhysicsWorld() const;
    void onFixedUpdate(float stepTime);
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
