#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "PhysicsWorld.h"
#include "Component.h"

class PhysicsToTransformTranslator
{
public:
    virtual ~PhysicsToTransformTranslator() {}
    virtual void translate() = 0;
};

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(const PhysicsWorld &world) :
        m_world(world.m_world.get()) {}
    virtual ~PhysicsComponent() {
    }
    virtual void onFixedUpdate(double stepTime) = 0;

protected:
    b2World *m_world;
    std::unique_ptr<PhysicsToTransformTranslator> m_translator;
};

#endif /* PHYSICS_COMPONENT_H */
