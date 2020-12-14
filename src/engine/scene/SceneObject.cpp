#include "SceneObject.h"
#include "Scene.h"
#include "RenderableComponent.h"
#include "PhysicsComponent.h"
#include "ControllerComponent.h"
#include "Transforms.h"

#include <cassert>

SceneObject::SceneObject(Scene *scene)
{
    assert(scene != nullptr);
    scene->addObject(this);
    m_scene = scene;
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

void SceneObject::updatePhysics(double stepTime)
{
    if (m_physicsComponent) {
        m_physicsComponent->onFixedUpdate(stepTime);
    }
}

void SceneObject::updateController(double stepTime)
{
    if (m_controllerComponent) {
        m_controllerComponent->onFixedUpdate(stepTime);
    }
}

void SceneObject::onKeyEvent(const Event::Key &keyEvent)
{
    if (m_controllerComponent) {
        m_controllerComponent->onKeyEvent(keyEvent);
    }
}

void SceneObject::onFixedUpdate(double stepTime)
{
    /* Do nothing */
}
