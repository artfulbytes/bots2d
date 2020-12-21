#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "PhysicsWorld.h"
#include "Component.h"

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(const PhysicsWorld &world) :
        m_world(world.m_world.get()) {}
    virtual ~PhysicsComponent() {
    }
    virtual void onFixedUpdate(double stepTime) = 0;

protected:
    class PhysicsToTransformTranslator
    {
    public:
        virtual void translate() = 0;
    };
    b2World *m_world;
    std::unique_ptr<PhysicsToTransformTranslator> m_translator;
};

#endif /* PHYSICS_COMPONENT_H */
