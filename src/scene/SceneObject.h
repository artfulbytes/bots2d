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

/**
 * Base class that scene objects inherit from. A scene object is a general purpose object. It's
 * similar to the "entity" in an Entity-component-system (ECS) pattern. It's an object composed
 * of multiple components such as rendering and physics components. In contrast to a real
 * ECS, there is no memory optimization, e.g. similar components are not stored next to each
 * other for cache-friendliness.
 *
 * A scene object must be part of a scene.
 */
class SceneObject
{
public:
    SceneObject(Scene *scene);
    virtual ~SceneObject();
    Scene *getScene() const { return m_scene; };
    void setController(ControllerComponent *controller);
    void updateRenderable();
    void updatePhysics(float stepTime);
    void updateController(float stepTime);
    virtual void onFixedUpdate(float stepTime);
    virtual void onKeyEvent(const Event::Key &keyEvent);

protected:
    Scene *m_scene = nullptr;
    PhysicsWorld *m_physicsWorld = nullptr;
    std::unique_ptr<TransformComponent> m_transformComponent;
    std::unique_ptr<RenderableComponent> m_renderableComponent;
    std::unique_ptr<PhysicsComponent> m_physicsComponent;
    /** Make controller a raw pointer because it's unflexible to have the scene object
     * own the controller. */
    ControllerComponent *m_controllerComponent = nullptr;
};

#endif /* SCENE_OBJECT_H_ */
