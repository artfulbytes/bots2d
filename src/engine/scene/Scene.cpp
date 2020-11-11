#include "Scene.h"
#include "SceneObject.h"
#include "PhysicsWorld.h"

void Scene::onKeyEvent(const Event::Key &keyEvent)
{
    /* TODO: How to make this closer to callback (now it's like >5 nested function calls) */
    /* TODO: Create an event list and process all press events between physics update and obj update */
    for (auto obj : m_objects) {
        obj->onKeyEvent(keyEvent);
    }
}

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
                         PhysicsComponent *physicsComp,
                         ControllerComponent *controllerComp)
{
    m_objects.push_back(new SceneObject(*this, transformComp, renderableComp, physicsComp, controllerComp));
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
