#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

class SceneObject;

class Scene
{
    public:
        Scene();
        ~Scene();
        void onUpdate();
        void addObject(SceneObject *obj);

    private:
        std::vector<SceneObject *> m_objects;
};

#endif /* SCENE_H_ */
