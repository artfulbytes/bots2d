#include "Scene.h"
#include "SceneObject.h"

Scene::Scene()
{
}

Scene::Scene(PhysicsWorld::Gravity gravity) :
    m_physicsWorld(std::make_unique<PhysicsWorld>(gravity))
{
}

Scene::~Scene()
{
}

PhysicsWorld *Scene::getPhysicsWorld() const
{
    assert(m_physicsWorld);
    return m_physicsWorld.get();
}

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
        obj->onFixedUpdate(stepTime);
    }

    for (auto obj : m_objects) {
        obj->updateRenderable();
    }
}

void Scene::addObject(SceneObject *sceneObject)
{
    assert(sceneObject != nullptr);
    assert(sceneObject->getScene() == nullptr);
    m_objects.push_back(sceneObject);
}
