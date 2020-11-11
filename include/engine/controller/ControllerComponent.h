#ifndef CONTROLLER_COMPONENT_H_
#define CONTROLLER_COMPONENT_H_

#include "Component.h"
/* TODO: Move to scene folder? */
class ControllerComponent : public Component
{
public:
    virtual void onKeyEvent(const Event::Key &keyEvent) = 0;
    virtual void onUpdate() = 0;
};

#endif /* CONTROLLER_COMPONENT_H_ */
