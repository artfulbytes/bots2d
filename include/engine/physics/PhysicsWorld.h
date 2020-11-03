#ifndef PHYSICS_WORLD_H_
#define PHYSICS_WORLD_H_

class b2World;

class PhysicsWorld
{
public:
    enum class Gravity { SideView, TopView, Custom };
    PhysicsWorld(Gravity gravity);
    PhysicsWorld(float gravityX, float gravityY);
    ~PhysicsWorld();

    void step(float timeStep);
    inline Gravity getGravityType() const { return m_gravityType; }

    /* We want physics components to have access to b2World */
    friend class PhysicsComponent;
private:
    b2World *m_world;
    Gravity m_gravityType = Gravity::SideView;

    /* TODO: Move scaling here (and make it adjustable?) */
};

#endif /* PHYSICS_WORLD_H_ */
