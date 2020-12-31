#ifndef SCALEBAR_H_
#define SCALEBAR_H_

#include "Texture.h"

/**
 * GUI-overlay at the bottom, which displays the scale. It adjusts to
 * the camera zoom factor.
 */
class Scalebar
{
public:
    Scalebar();
    ~Scalebar();
    void render();

private:
    struct ScalebarRenderable
    {
        ScalebarRenderable(std::string textureName, float width, float height);
        Texture texture;
        float width;
        float height;
    };

    const ScalebarRenderable m_scalebar4m;
    const ScalebarRenderable m_scalebar2m;
    const ScalebarRenderable m_scalebar1m;
    const ScalebarRenderable m_scalebar50cm;
    const ScalebarRenderable m_scalebar25cm;
};

#endif /* SCALEBAR_H */
