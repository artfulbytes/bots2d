#ifndef COMPONENT_H_
#define COMPONENT_H_

/**
 * The base class all implemented components indirectly inherit from.
 */
class Component
{
public:
    virtual ~Component() { }
    /**
     * Called every simulation iteration (if assigned to a Scene Object).
     */
    virtual void onFixedUpdate(float stepTime)
    {
        (void)stepTime;
    };
};

#endif /* COMPONENT_H_ */
