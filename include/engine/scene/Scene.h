#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

class TransformComponent;
class RenderableComponent;
class PhysicsComponent;
class PhysicsWorld;
class SceneObject;

class Scene
{
public:
    Scene();
    ~Scene();
    void onUpdate();
    void createObject(TransformComponent *transformComp,
                      RenderableComponent *renderableComp,
                      PhysicsComponent *physicsComp);
    void setPhysicsWorld(PhysicsWorld *world);
    const PhysicsWorld *getPhysicsWorld() const { return m_physicsWorld; }

private:
    std::vector<SceneObject *> m_objects;

protected:
    PhysicsWorld *m_physicsWorld = nullptr;
};

#endif /* SCENE_H_ */
