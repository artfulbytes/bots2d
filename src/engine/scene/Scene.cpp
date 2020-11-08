#include "Scene.h"
#include "SceneObject.h"
#include "PhysicsWorld.h"

void Scene::onUpdate()
{
    if (m_physicsWorld) {
        m_physicsWorld->step(0.01);
        for (auto obj : m_objects) {
            obj->updatePhysics();
        }
    }

    for (auto obj : m_objects) {
        obj->render();
    }
}

void Scene::createObject(TransformComponent *transformComp,
                         RenderableComponent *renderableComp,
                         PhysicsComponent *physicsComp)
{
    m_objects.push_back(new SceneObject(*this, transformComp, renderableComp, physicsComp));
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
