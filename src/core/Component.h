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
     * Called every physics step (if assigned to a Scene Object).
     */
    virtual void onFixedUpdate()
    {
    };
};

#endif /* COMPONENT_H_ */
