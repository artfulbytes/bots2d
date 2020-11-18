#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "PhysicsWorld.h"
#include "Component.h"

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(const PhysicsWorld &world) :
        m_world(world.m_world) {}
    ~PhysicsComponent() {
        delete m_translator;
    }
    virtual void onFixedUpdate(double stepTime) = 0;

protected:
    /* TODO: Is there a cleaner way than introducinga translator class here? */
    class PhysicsToTransformTranslator
    {
        public: virtual void translate() = 0;
    };
    b2World *m_world = nullptr;
    PhysicsToTransformTranslator *m_translator = nullptr;
};

#endif /* PHYSICS_COMPONENT_H */
