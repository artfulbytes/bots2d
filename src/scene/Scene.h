#ifndef SCENE_H_
#define SCENE_H_

#include "Event.h"
#include "PhysicsWorld.h"
#include <vector>
#include <memory>
#include <string>
#include <chrono>

class SceneObject;
class ImGuiMenu;

/**
 * Base class for scenes. All scenes must inherit this class. A Scene provides the stage
 * for the simulation and has a PhysicsWorld and a list of SceneObject. Only one Scene
 * can be active at a time.
 */
class Scene
{
public:
    Scene(std::string description);
    /**
    * \param physicsStepTime Determines how many times per second the physics and logic are updated.
    * Note, to avoid jittery behaviour, make this at least twice as large as the rendering rate
    * (monitor's refresh rate).
    */
    Scene(std::string description, PhysicsWorld::Gravity gravity, float physicsStepTime = 0.001f);
    virtual ~Scene();
    PhysicsWorld *getPhysicsWorld() const;
    void updatePhysics(float stepTime);
    void updateControllers(float stepTime);
    void sceneObjectsOnFixedUpdate();
    void render();
    void onKeyEvent(const Event::Key &keyEvent);
    void addObject(SceneObject *sceneObject);
    void removeObject(SceneObject *sceneObject);
    void addMenu(ImGuiMenu *menu);
    std::string getDescription() const { return m_description; }
    unsigned int getSecondsSinceStart() const;
    unsigned int getMillisecondsSinceStart() const;
    float getPhysicsStepTime() const { return m_physicsStepTime; }

protected:
    std::unique_ptr<PhysicsWorld> m_physicsWorld;

private:
    std::vector<SceneObject *> m_objects;
    std::vector<ImGuiMenu *> m_menus;
    std::string m_description;
    float m_physicsStepTime = 0.001f;
    const std::chrono::time_point<std::chrono::system_clock> m_startTime;
};

#endif /* SCENE_H_ */
