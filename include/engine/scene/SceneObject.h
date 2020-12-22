#ifndef SCENE_OBJECT_H_
#define SCENE_OBJECT_H_

#include "Event.h"
#include <memory>

class TransformComponent;
class RenderableComponent;
class PhysicsComponent;
class ControllerComponent;
class Scene;
class PhysicsWorld;

class SceneObject
{
public:
    /* A scene object must be part of a scene */
    SceneObject(Scene *scene);
    virtual ~SceneObject();
    Scene *getScene() const { return m_scene; };
    void setController(ControllerComponent *controller);
    void updateRenderable();
    void updatePhysics(double stepTime);
    void updateController(double stepTime);
    virtual void onFixedUpdate(double stepTime);
    virtual void onKeyEvent(const Event::Key &keyEvent);

protected:
    Scene *m_scene = nullptr;
    PhysicsWorld *m_physicsWorld = nullptr;
    /* This is not cache friendly, it would be more optimal to use an entity component system.
     * But the simulations are still simple so let's not over-engineer it. */
    std::unique_ptr<TransformComponent> m_transformComponent;
    std::unique_ptr<RenderableComponent> m_renderableComponent;
    std::unique_ptr<PhysicsComponent> m_physicsComponent;
    /* Make controller a raw pointer because it's unflexible to have the scene object
     * own the controller. */
    ControllerComponent *m_controllerComponent = nullptr;
};

#endif /* SCENE_OBJECT_H_ */
