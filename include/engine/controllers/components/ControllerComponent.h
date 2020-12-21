#ifndef CONTROLLER_COMPONENT_H_
#define CONTROLLER_COMPONENT_H_

#include "Component.h"
#include "Event.h"

class ControllerComponent : public Component
{
public:
    virtual void onKeyEvent(const Event::Key &keyEvent) = 0;
    virtual void onFixedUpdate(double stepTime) = 0;
    virtual ~ControllerComponent() {}
};

#endif /* CONTROLLER_COMPONENT_H_ */
