#ifndef SPRITE_ANIMATION_H_
#define SPRITE_ANIMATION_H_

#include "Vec2.h"
#include "TexCoords.h"

class SpriteAnimation {
public:
    /* NOTE: Certain values of spriteCountAxis (spritesheet row/column) lead to small rounding
     * errors of the texture coordinates, which results in a slighty miss-aligned sprite. */
    SpriteAnimation(const Vec2<unsigned int> &spriteCountAxis, unsigned int spriteCount,
                    unsigned int framesBetweenUpdates);
    ~SpriteAnimation();
    void setFramesBetweenUpdates(unsigned int framesBetweenUpdates);
    void onFixedUpdate();
    void getTexCoords(TexCoords &texCoords) const;
    void computeTexCoords(TexCoords &texCoords) const;

private:
    /* Specifies how many columns and rows in the sprite sheet */
    const Vec2<unsigned int> m_spriteCountAxis = {1, 1};
    const unsigned int m_spriteCount = 1;
    unsigned int m_currentSpriteIndex = 1;
    unsigned int m_framesBetweenUpdates = 10;
    unsigned int m_framesSinceLastUpdate = 0;
    const float m_spriteHeight = 1.0f;
    const float m_spriteWidth = 1.0f;
    TexCoords m_texCoords;
};

#endif /* SPRITE_ANIMATION_H_ */
