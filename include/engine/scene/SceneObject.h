#ifndef SCENE_OBJECT_H_
#define SCENE_OBJECT_H_

#include "Event.h"

class TransformComponent;
class RenderableComponent;
class PhysicsComponent;
class ControllerComponent;
class Scene;

class SceneObject
{
    public:
        /* A scene object must be part of a scene */
        SceneObject(const Scene &scene,
                    TransformComponent *transformComp,
                    RenderableComponent *renderableComp,
                    PhysicsComponent *physicsComp,
                    ControllerComponent *controllerComp);
        ~SceneObject();
        inline TransformComponent *getTransform() { return m_transformComp; }
        /* TODO: Pass renderable component to scene instead? */
        void render();
        void updatePhysics();
        void updateController();
        void onKeyEvent(const Event::Key &keyEvent);

    private:
        const Scene *m_scene;
        /* TODO: Make this a general array of components instead. Passing all components
         * Or create a struct you pass
         * via constructor is getting out of hand.
         * This is not cache friendly, it would be more optimal to use an entity component system.
         * But the simulations are still simple so let's not over-engineer it. */
        TransformComponent *m_transformComp = nullptr;
        RenderableComponent *m_renderableComp = nullptr;
        PhysicsComponent *m_physicsComp = nullptr;
        ControllerComponent *m_controllerComp = nullptr;
};

#endif /* SCENE_OBJECT_H_ */
