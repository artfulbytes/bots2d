#include "SpriteAnimation.h"

namespace {
    float maxFloatRoundingError = 0.001f;
}

SpriteAnimation::SpriteAnimation(const Vec2<unsigned int> &spriteCountAxis, const unsigned int spriteCount, unsigned int framesBetweenUpdates,
                                SpriteAnimation::Direction animationDirection) :
    m_spriteCountAxis(spriteCountAxis),
    m_spriteCount(spriteCount),
    m_framesBetweenUpdates(framesBetweenUpdates),
    m_spriteWidth(1.0f / m_spriteCountAxis.x),
    m_spriteHeight(1.0f / m_spriteCountAxis.y),
    m_animationDirection(animationDirection)
{
    if (animationDirection == SpriteAnimation::Direction::Backward) {
        m_currentSpriteIndex = m_spriteCount - 1;
    }
}

SpriteAnimation::~SpriteAnimation()
{
}

void SpriteAnimation::stop()
{
    m_stopped = true;
}

void SpriteAnimation::setDirection(Direction animationDirection)
{
    m_animationDirection = animationDirection;
}

void SpriteAnimation::setFramesBetweenUpdates(unsigned int framesBetweenUpdates)
{
    m_stopped = false;
    m_framesBetweenUpdates = framesBetweenUpdates;
}

void SpriteAnimation::onFixedUpdate()
{
    if (m_stopped) {
        return;
    }
    if (m_framesSinceLastUpdate != m_framesBetweenUpdates) {
        m_framesSinceLastUpdate++;
        return;
    }
    if (m_animationDirection == SpriteAnimation::Direction::Forward) {
        m_currentSpriteIndex = (m_currentSpriteIndex + 1) % m_spriteCount;
    } else {
        if (m_currentSpriteIndex == 0) {
            m_currentSpriteIndex = m_spriteCount - 1;
        } else {
            m_currentSpriteIndex--;
        }
    }
    m_framesSinceLastUpdate = 0;
}

void SpriteAnimation::computeTexCoords(TexCoords &texCoords) const
{
    const Vec2<unsigned int> m_currentSprite = {m_currentSpriteIndex % m_spriteCountAxis.x, m_currentSpriteIndex / m_spriteCountAxis.x};
    texCoords.TopLeft.x = m_currentSprite.x * m_spriteWidth;
    texCoords.TopLeft.y = 1.0f - m_currentSprite.y * m_spriteHeight;
    if (texCoords.TopLeft.y < 0.0f)
    {
        assert(texCoords.TopLeft.y >= -maxFloatRoundingError);
        texCoords.TopLeft.y = 0.0f;
    }
    texCoords.TopRight.x = texCoords.TopLeft.x + m_spriteWidth;
    texCoords.TopRight.y = texCoords.TopLeft.y;
    texCoords.BottomLeft.x = texCoords.TopLeft.x;
    texCoords.BottomLeft.y = texCoords.TopLeft.y - m_spriteHeight;
    if (texCoords.BottomLeft.y < 0.0f)
    {
        assert(texCoords.BottomLeft.y >= -maxFloatRoundingError);
        texCoords.BottomLeft.y = 0.0f;
    }
    texCoords.BottomRight.x = texCoords.TopLeft.x + m_spriteWidth;
    texCoords.BottomRight.y = texCoords.BottomLeft.y;
}
