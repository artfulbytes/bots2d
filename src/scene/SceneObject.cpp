#include "SceneObject.h"
#include "Scene.h"
#include "components/RenderableComponent.h"
#include "components/PhysicsComponent.h"
#include "components/ControllerComponent.h"
#include "components/Transforms.h"

#include <cassert>

SceneObject::SceneObject(Scene *scene)
{
    assert(scene != nullptr);
    scene->addObject(this);
    m_scene = scene;
    m_physicsWorld = scene->getPhysicsWorld();
}

SceneObject::~SceneObject()
{
}

void SceneObject::setController(ControllerComponent *controller)
{
    assert(controller != nullptr);
    m_controllerComponent = controller;
}

void SceneObject::updateRenderable()
{
    if (m_renderableComponent) {
        m_renderableComponent->onFixedUpdate();
    }
}

void SceneObject::updatePhysics()
{
    if (m_physicsComponent) {
        m_physicsComponent->onFixedUpdate();
    }
}

void SceneObject::updateController()
{
    if (m_controllerComponent) {
        m_controllerComponent->onFixedUpdate();
    }
}

void SceneObject::onKeyEvent(const Event::Key &keyEvent)
{
    if (m_controllerComponent) {
        m_controllerComponent->onKeyEvent(keyEvent);
    }
}

void SceneObject::onFixedUpdate()
{
    /* Do nothing */
}
