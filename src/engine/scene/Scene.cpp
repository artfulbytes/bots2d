#include "Scene.h"
#include "SceneObject.h"
#include "PhysicsWorld.h"

void Scene::onUpdate()
{
    if (m_physicsWorld) {
        m_physicsWorld->step(0.01);
        /* This is not cache friendly, optimally we should use an entity component
        * system here, but since we render few components it's okay for now. */
        for (auto obj : m_objects) {
            obj->updatePhysics();
        }
    }

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

void Scene::setPhysicsWorld(PhysicsWorld *world) {
    m_physicsWorld = world;
}

Scene::Scene()
{
}

Scene::~Scene()
{
    for (auto obj : m_objects) {
        delete obj;
    }

    delete m_physicsWorld;
}
