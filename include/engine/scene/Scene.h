#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

class SceneObject;
class PhysicsWorld;

class Scene
{
    public:
        Scene();
        ~Scene();
        void onUpdate();
        void addObject(SceneObject *obj);
        void setPhysicsWorld(PhysicsWorld *world);

    private:
        std::vector<SceneObject *> m_objects;

    protected:
        PhysicsWorld *m_physicsWorld;
};

#endif /* SCENE_H_ */
