#ifndef SPRITE_ANIMATION_H_
#define SPRITE_ANIMATION_H_

#include "TexCoords.h"

class SpriteAnimation {
public:
    enum class Direction { Forward, Backward };
    struct Params {
        unsigned int spriteSheetWidth = 0;
        unsigned int spriteSheetHeight = 0;
        unsigned int spriteCount = 0;
        unsigned int framesBetweenUpdates = 0;
        Direction direction = Direction::Forward;
    };
    /* NOTE: Certain values of spriteCountAxis (spritesheet row/column) lead to small rounding
     * errors of the texture coordinates, which results in a slighty miss-aligned sprite. */
    SpriteAnimation(unsigned int spriteSheetWidth, unsigned int spriteSheetHeight, unsigned int spriteCount,
                    unsigned int framesBetweenUpdates, Direction animationDirection = Direction::Forward);
    SpriteAnimation(const Params &params);
    ~SpriteAnimation();
    void setFramesBetweenUpdates(unsigned int framesBetweenUpdates);
    void setDirection(Direction animationDirection);
    void stop();
    void onFixedUpdate();
    void getTexCoords(TexCoords &texCoords) const;
    void computeTexCoords(TexCoords &texCoords) const;

private:
    unsigned int m_spriteSheetWidth = 1;
    unsigned int m_spriteSheetHeight = 1;
    const unsigned int m_spriteCount = 1;
    unsigned int m_currentSpriteIndex = 1;
    unsigned int m_framesBetweenUpdates = 10;
    unsigned int m_framesSinceLastUpdate = 0;
    const float m_spriteHeight = 1.0f;
    const float m_spriteWidth = 1.0f;
    TexCoords m_texCoords;
    Direction m_animationDirection = Direction::Forward;
    bool m_stopped = false;
};

#endif /* SPRITE_ANIMATION_H_ */
