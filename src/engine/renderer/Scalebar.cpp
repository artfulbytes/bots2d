#include "Scalebar.h"
#include "Renderer.h"
#include "Camera.h"


Scalebar::ScalebarRenderable::ScalebarRenderable(std::string textureName, float width, float height) :
    texture(textureName),
    width(width),
    height(height)
{
}

Scalebar::Scalebar() :
    m_scalebar4m("scalebar4m.png", 4.0f, 0.18f),
    m_scalebar2m("scalebar2m.png", 2.0f, 0.09f),
    m_scalebar1m("scalebar1m.png", 1.0f, 0.045f),
    m_scalebar50cm("scalebar50cm.png", 0.50f, 0.0225f),
    m_scalebar25cm("scalebar25cm.png", 0.25f, 0.01125f)
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
    const float extraBottomMargin = 0.1f * scalebar->height;
    const float bottomMargin = (pixelScaleFactor * zoomFactor) * ((scalebar->height / 2.0f) + extraBottomMargin);
    const float positionY = bottomMargin - cameraPosition.y;
    const float positionX = (windowSize.x / 2.0f) - cameraPosition.x;

    glm::vec2 drawPosition = glm::vec2{positionX, positionY} / (pixelScaleFactor * zoomFactor);
    glm::vec2 drawSize { scalebar->width, scalebar->height };

    if ((drawSize.x * zoomFactor * pixelScaleFactor) > windowSize.x) {
        /* Hide if scalebar is larger than window */
        return;
    }

    Renderer::drawRect(drawPosition, drawSize, 0.0f, scalebar->texture);
}
