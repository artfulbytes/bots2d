#include "Scene.h"
#include "SceneObject.h"
#include "PhysicsWorld.h"

void Scene::onKeyEvent(const Event::Key &keyEvent)
{
    for (auto obj : m_objects) {
        obj->onKeyEvent(keyEvent);
    }
}

void Scene::onFixedUpdate(double stepTime)
{
    if (m_physicsWorld) {
        m_physicsWorld->step(stepTime);
        for (auto obj : m_objects) {
            obj->updatePhysics(stepTime);
        }
    }

    for (auto obj : m_objects) {
        obj->updateController(stepTime);
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
