#include "Scene.h"
#include "SceneObject.h"
#include "ImGuiMenu.h"

Scene::Scene(std::string description) :
    m_description(description)
{
}

Scene::Scene(std::string description, PhysicsWorld::Gravity gravity, float physicsStepTime) :
    m_physicsWorld(std::make_unique<PhysicsWorld>(gravity)),
    m_description(description),
    m_physicsStepTime(physicsStepTime),
    m_startTime(std::chrono::system_clock::now())
{
}

Scene::~Scene()
{
}

PhysicsWorld *Scene::getPhysicsWorld() const
{
    return m_physicsWorld.get();
}

void Scene::onKeyEvent(const Event::Key &keyEvent)
{
    for (auto obj : m_objects) {
        obj->onKeyEvent(keyEvent);
    }
}

void Scene::updatePhysics(float stepTime)
{
    if (m_physicsWorld) {
        m_physicsWorld->step(stepTime);
        for (auto obj : m_objects) {
            obj->updatePhysics();
        }
    }
}

void Scene::updateControllers()
{
    for (auto obj : m_objects) {
        obj->updateController();
    }
}

void Scene::sceneObjectsOnFixedUpdate()
{
    for (auto obj : m_objects) {
        obj->onFixedUpdate();
    }
}

void Scene::render()
{
    for (auto menu : m_menus) {
        menu->render();
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

void Scene::removeObject(SceneObject *sceneObject)
{
    auto itr = m_objects.begin();
    while (itr != m_objects.end())
    {
        if (sceneObject == *itr) {
            itr = m_objects.erase(itr);
            break;
        }
        itr++;
    }
}

void Scene::addMenu(ImGuiMenu *menu)
{
    assert(menu != nullptr);
    m_menus.push_back(menu);
}

unsigned int Scene::getSecondsSinceStart() const
{
    const auto timeNow = std::chrono::system_clock::now();
    return std::chrono::duration<double>(timeNow - m_startTime).count();
}
