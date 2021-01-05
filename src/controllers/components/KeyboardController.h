#ifndef KEYBOARD_CONTROLLER_H_
#define KEYBOARD_CONTROLLER_H_

#include "ControllerComponent.h"

/**
 * Base class for classes that controls a SceneObject based on keyboard input.
 */
class KeyboardController : public ControllerComponent
{
public:
    virtual void onKeyEvent(const Event::Key &keyEvent) = 0;
    virtual void onFixedUpdate(float stepTime) = 0;
};

#endif /* KEYBOARD_CONTROLLER_H_ */
