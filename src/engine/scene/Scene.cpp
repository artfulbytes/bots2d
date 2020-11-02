#include "Scene.h"
#include "SceneObject.h"

void Scene::onUpdate()
{
    /* This is not cache friendly, optimally we should use an entity component
     * system here, but since we render few components it's okay for now. */
     for (auto obj : m_objects) {
         obj->render();
     }
}

void Scene::addObject(SceneObject *obj)
{
    m_objects.push_back(obj);
}

Scene::Scene()
{
}

Scene::~Scene()
{
    for (auto obj : m_objects) {
        delete obj;
    }
}
