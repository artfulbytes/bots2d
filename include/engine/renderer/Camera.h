#ifndef CAMERA_H_
#define CAMERA_H_

#include "Event.h"
#include <glm/glm.hpp>

/**
 * Camera position and zoom factor.
 */
class Camera
{
public:
    static bool onKeyEvent(const Event::Key &keyEvent);
    static void onScrollEvent(const Event::Scroll &scrollEvent);
    static void onWindowEvent(const Event::Window &windowEvent);
    static float getZoomFactor();
    static glm::vec2 getPosition();
    static glm::vec2 getWindowSize();
    static void reset();
};

#endif /* CAMERA_H_ */
