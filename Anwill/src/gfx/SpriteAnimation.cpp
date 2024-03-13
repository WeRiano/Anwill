#include "gfx/SpriteAnimation.h"
#include "core/Log.h"

namespace Anwill {

    SpriteAnimation::SpriteAnimation(const Timestamp& frameDelta)
        : m_FrameDelta(frameDelta), m_Current(0)
    {}

    void SpriteAnimation::Tick(const Timestamp& delta)
    {
        m_Current += delta;
        if (m_Current >= m_FrameDelta) {
            m_Current -= m_FrameDelta;

            auto sprite = m_FrameQ.front();
            m_FrameQ.pop();
            m_FrameQ.push(sprite);
        }
    }

    void SpriteAnimation::AddFrame(const std::shared_ptr<Texture>& texture,
                                   const QuadTexCoords& texCoords)
    {
        m_FrameQ.emplace(texture, texCoords);
    }

    void SpriteAnimation::AddFrame(const std::shared_ptr<SpriteSheet>& spriteSheet,
                                   unsigned int spriteSheetXPos,
                                   unsigned int spriteSheetYPos,
                                   int leftPadPixels, int rightPadPixels,
                                   int bottomPadPixels, int topPadPixels)
    {
        AddFrame(spriteSheet->GetSprite(spriteSheetXPos,
                                            spriteSheetYPos,
                                            leftPadPixels, rightPadPixels,
                                            bottomPadPixels, topPadPixels));
    }

    void SpriteAnimation::AddFrame(const Sprite& sprite)
    {
        m_FrameQ.push(sprite);
    }

    void SpriteAnimation::AddFrames(const std::shared_ptr<SpriteSheet>& spriteSheet, unsigned int startXPos,
                                    unsigned int startYPos, unsigned int xIncrement, unsigned int yIncrement,
                                    unsigned int count)
    {
        unsigned int i = 0;
        while(i < count) {
            AddFrame(spriteSheet->GetSprite(startXPos, startYPos, 0, 0, 0, 0));
            startXPos += xIncrement;
            startYPos += yIncrement;
            i++;
        }
    }

    void SpriteAnimation::ChangeFrameDelta(const Timestamp& delta,
                                           const Timestamp& min,
                                           const Timestamp& max)
    {
        m_FrameDelta += delta;
        if(m_FrameDelta < min) {
            m_FrameDelta = min;
        } else if(m_FrameDelta > max) {
            m_FrameDelta = max;
        }
    }

    void SpriteAnimation::ChangeFrameDelta(double delta, const Timestamp& max)
    {
        m_FrameDelta += (m_FrameDelta * delta);
        if(m_FrameDelta > max) {
            m_FrameDelta = max;
        }
    }

    Sprite SpriteAnimation::GetActiveFrame() const
    {
        return m_FrameQ.front();
    }

    void SpriteAnimation::SetFrameDelta(const Timestamp& frameDelta)
    {
        m_FrameDelta = frameDelta;
    }
}

