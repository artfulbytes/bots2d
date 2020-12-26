#include "Scalebar.h"
#include "Renderer.h"
#include "Camera.h"

Scalebar::Scalebar() :
    m_scalebar4m("scalebar4m.png", 40.0f, 1.8f),
    m_scalebar2m("scalebar2m.png", 20.0f, 0.9f),
    m_scalebar1m("scalebar1m.png", 10.0f, 0.45f),
    m_scalebar50cm("scalebar50cm.png", 5.0f, 0.225f),
    m_scalebar25cm("scalebar25cm.png", 2.5f, 0.1125f)
{
}

Scalebar::~Scalebar()
{
}

void Scalebar::render()
{
    const float pixelScaleFactor = Renderer::getPixelScaleFactor();
    const glm::vec2 cameraPosition = Camera::getPosition();
    const float zoomFactor = Camera::getZoomFactor();
    const glm::vec2 windowSize = Camera::getWindowSize();

    const ScalebarRenderable *scalebar = nullptr;
    if (zoomFactor < 0.25f) {
        scalebar = &m_scalebar4m;
    } else if (zoomFactor < 0.5f) {
        scalebar = &m_scalebar2m;
    } else if (zoomFactor < 1.0f) {
        scalebar = &m_scalebar1m;
    } else if (zoomFactor < 2.0f) {
        scalebar = &m_scalebar50cm;
    } else {
        scalebar = &m_scalebar25cm;
    }
    const float bottomMargin = (pixelScaleFactor * zoomFactor) * (scalebar->height / 2.0f);
    const float positionY = bottomMargin - cameraPosition.y;
    const float positionX = (windowSize.x / 2.0f) - cameraPosition.x;

    glm::vec2 drawPosition = glm::vec2{positionX, positionY} / (pixelScaleFactor * zoomFactor);
    glm::vec2 drawSize { scalebar->width, scalebar->height };

    Renderer::drawQuad(drawPosition, drawSize, 0.0f, scalebar->texture);
}
