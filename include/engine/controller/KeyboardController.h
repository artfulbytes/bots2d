#ifndef KEYBOARD_CONTROLLER_H_
#define KEYBOARD_CONTROLLER_H_

#include "ControllerComponent.h"

class KeyboardController : public ControllerComponent
{
public:
    virtual void onKeyEvent(const Event::Key &keyEvent) = 0;
    virtual void onUpdate() = 0;
};

#endif /* KEYBOARD_CONTROLLER_H_ */
