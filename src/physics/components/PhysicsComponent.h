#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "PhysicsWorld.h"
#include "Component.h"

/**
 * Base class for translating from physics data to transform data.
 * Physics component classes should use inherited classes of this
 * when they update transforms.
 */
class PhysicsToTransformTranslator
{
public:
    virtual ~PhysicsToTransformTranslator() {}
    virtual void translate() = 0;
};

/**
 * Base class for components that implement physics behaviour.
 *
 * It can't be used directly in a Scene, instead it must be assigned to
 * a Scene object to be updated each simulation iteration.
 */
class PhysicsComponent : public Component
{
public:
    PhysicsComponent(const PhysicsWorld &world) :
        m_world(world.m_world.get()) {}
    virtual ~PhysicsComponent() {
    }
    virtual void onFixedUpdate(float stepTime) = 0;

protected:
    b2World *m_world;
    std::unique_ptr<PhysicsToTransformTranslator> m_translator;
};

#endif /* PHYSICS_COMPONENT_H */
