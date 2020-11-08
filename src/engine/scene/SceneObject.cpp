#include "SceneObject.h"
#include "Scene.h"
#include "RenderableComponent.h"
#include "PhysicsComponent.h"
#include "Transforms.h"

#include <cassert>

SceneObject::SceneObject(const Scene &scene,
                         TransformComponent *transformComp,
                         RenderableComponent *renderableComp,
                         PhysicsComponent *physicsComp) :
    m_scene(&scene),
    m_transformComp(transformComp),
    m_renderableComp(renderableComp),
    m_physicsComp(physicsComp)
{
    /* SceneObject must have a transform */
    assert(m_transformComp);
    m_transformComp->m_parent = this;

    if (m_renderableComp) {
        m_renderableComp->m_parent = this;
    }

    if (m_physicsComp) {
        m_physicsComp->m_parent = this;
    }
}

SceneObject::~SceneObject()
{
    delete m_transformComp;
    delete m_renderableComp;
    delete m_physicsComp;
}

void SceneObject::render()
{
    if (m_renderableComp) {
        m_renderableComp->render();
    }
}

void SceneObject::updatePhysics()
{
    if (m_physicsComp) {
        m_physicsComp->update();
    }
}
