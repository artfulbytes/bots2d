#include "SceneObject.h"
#include "Scene.h"
#include "RenderableComponent.h"
#include "PhysicsComponent.h"
#include "ControllerComponent.h"
#include "Transforms.h"

#include <cassert>

SceneObject::SceneObject(const Scene &scene,
                         TransformComponent *transformComp,
                         RenderableComponent *renderableComp,
                         PhysicsComponent *physicsComp,
                         ControllerComponent *controllerComp) :
    m_scene(&scene),
    m_transformComp(transformComp),
    m_renderableComp(renderableComp),
    m_physicsComp(physicsComp),
    m_controllerComp(controllerComp)
{
    if (m_transformComp) {
        m_transformComp->m_parent = this;
    }

    if (m_renderableComp) {
        assert(m_transformComp);
        m_renderableComp->m_parent = this;
    }

    if (m_physicsComp) {
        assert(m_transformComp);
        m_physicsComp->m_parent = this;
    }

    if (m_controllerComp) {
        m_controllerComp->m_parent = this;
    }
}

SceneObject::~SceneObject()
{
    delete m_transformComp;
    delete m_renderableComp;
    delete m_physicsComp;
    delete m_controllerComp;
}

void SceneObject::render()
{
    if (m_renderableComp) {
        /* TODO: Change to onUpdate? */
        m_renderableComp->render();
    }
}

void SceneObject::updatePhysics(double stepTime)
{
    if (m_physicsComp) {
        /* TODO: Change to onUpdate? */
        m_physicsComp->update();
    }
}

void SceneObject::updateController(double stepTime)
{
    if (m_controllerComp) {
        m_controllerComp->onFixedUpdate(stepTime);
    }
}

void SceneObject::onKeyEvent(const Event::Key &keyEvent)
{
    if (m_controllerComp) {
        m_controllerComp->onKeyEvent(keyEvent);
    }
}
