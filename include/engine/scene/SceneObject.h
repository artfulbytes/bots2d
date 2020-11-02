#ifndef SCENE_OBJECT_H_
#define SCENE_OBJECT_H_

class TransformComponent;
class RenderableComponent;
class PhysicsComponent;

class SceneObject
{
    public:
        SceneObject(TransformComponent *transformComp,
               RenderableComponent *renderableComp,
               PhysicsComponent *physicsComp);
        ~SceneObject();
        inline TransformComponent *getTransform() { return m_transformComp; }
        /* TODO: Pass renderable component to scene instead? */
        void render();

    private:
        /* TODO: Make this a general array of components instead? */
        TransformComponent *m_transformComp = nullptr;
        RenderableComponent *m_renderableComp = nullptr;
        PhysicsComponent *m_physicsComp = nullptr;
};

#endif /* SCENE_OBJECT_H_ */
