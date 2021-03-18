#ifndef CONTROLLER_COMPONENT_H_
#define CONTROLLER_COMPONENT_H_

#include "Component.h"
#include "Event.h"

/**
 * Base class for components that implement control behaviour based on
 * key events or pure logic.
 */
class ControllerComponent : public Component
{
public:
    virtual void onKeyEvent(const Event::Key &keyEvent) = 0;
    virtual void onFixedUpdate(float stepTime) = 0;
    virtual void resume() {};
    virtual ~ControllerComponent() {}
};

#endif /* CONTROLLER_COMPONENT_H_ */
