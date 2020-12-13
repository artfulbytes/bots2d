#ifndef CAMERA_H_
#define CAMERA_H_

#include "Event.h"

class Camera
{
public:
    static bool onKeyEvent(const Event::Key &keyEvent);
    static void onScrollEvent(const Event::Scroll &scrollEvent);
    static void onWindowEvent(const Event::Window &windowEvent);
    static void reset();
};

#endif /* CAMERA_H_ */
