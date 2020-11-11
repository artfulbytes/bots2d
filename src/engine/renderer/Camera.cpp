#include "Camera.h"
#include "Renderer.h"

/* TODO: Init static variables like this? */
namespace {
    const float positionStepSize = 1.0f;
    const float zoomStepSize = 1.25f;
    const float minZoomFactor = 0.1f;
    const float maxZoomFactor = 4.0f;

    glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};
    float zoomFactor = 1.0f;
}

bool Camera::onKeyEvent(const Event::Key &keyEvent)
{
    bool changed = false;
    if (keyEvent.action == Event::KeyAction::Press || keyEvent.action == Event::KeyAction::Repeat) {
        switch (keyEvent.code)
        {
        case Event::KeyCode::A:
            cameraPosition.x += positionStepSize;
            break;
        case Event::KeyCode::D:
            cameraPosition.x -= positionStepSize;
            break;
        case Event::KeyCode::S:
            cameraPosition.y += positionStepSize;
            break;
        case Event::KeyCode::W:
            cameraPosition.y -= positionStepSize;
            break;
        default:
            return false;
        }
        changed = true;
    }

    if (changed) {
        Renderer::setCameraPosition(cameraPosition, zoomFactor);
        return true;
    }
}

void Camera::onScrollEvent(const Event::Scroll &scrollEvent)
{
    if (scrollEvent.yoffset > 0) {
        /* Zoom in */
        zoomFactor *= zoomStepSize;
        zoomFactor = zoomFactor > maxZoomFactor ? maxZoomFactor : zoomFactor;
    } else if (scrollEvent.yoffset < 0) {
        /* Zoom out */
        zoomFactor *= (1 / zoomStepSize);
        zoomFactor = zoomFactor < minZoomFactor ? minZoomFactor : zoomFactor;
    }
    Renderer::setCameraPosition(cameraPosition, zoomFactor);
}

void Camera::onWindowEvent(const Event::Window &windowEvent)
{
    Renderer::setViewport(0, 0, windowEvent.width, windowEvent.height);
}
