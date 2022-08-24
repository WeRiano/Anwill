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
        if (m_TexToID.contains(texture))
        {
            unsigned int id = m_TexToID[texture];
            m_FrameQ.emplace(id, texCoords);
        } else
        {
            unsigned int newID = m_FrameQ.size();
            m_FrameQ.emplace(newID, texCoords);
            m_IDToTex[newID] = texture;
            m_TexToID[texture] = newID;
        }
    }

    void SpriteAnimation::AddFrame(const std::shared_ptr<SpriteSheet>& spriteSheet,
                                   unsigned int spriteSheetXPos,
                                   unsigned int spriteSheetYPos,
                                   int pixelLeftPad, int pixelRightPad,
                                   int pixelBottomPad, int pixelTopPad)
    {
        auto texCoords = spriteSheet->GetEvenSpriteTexCoords(spriteSheetXPos,
                                                             spriteSheetYPos,
                                                             pixelLeftPad, pixelRightPad,
                                                             pixelBottomPad, pixelTopPad);
        AddFrame(spriteSheet->GetTexture(), texCoords);
    }

    void SpriteAnimation::AddFramesHorizontally(
            const std::shared_ptr<SpriteSheet>& spriteSheet, unsigned int startXPos,
            unsigned int startYPos, unsigned int increment, unsigned int count)
    {
        unsigned int y = startYPos;
        for(unsigned int x = startXPos; x < startXPos + count; x += increment)
        {
            auto texCoords = spriteSheet->GetEvenSpriteTexCoords(x, y, 0, 0, 0, 0);
            AddFrame(spriteSheet->GetTexture(), texCoords);
        }
    }

    void
    SpriteAnimation::AddFramesVertically(const std::shared_ptr<SpriteSheet>& spriteSheet,
                                         unsigned int startXPos, unsigned int startYPos,
                                         unsigned int increment, unsigned int count)
    {
        unsigned int x = startXPos;
        for(unsigned int y = startYPos; y < startXPos + count; y += increment)
        {
            auto texCoords = spriteSheet->GetEvenSpriteTexCoords(x, y, 0, 0, 0, 0);
            AddFrame(spriteSheet->GetTexture(), texCoords);
        }
    }

    void
    SpriteAnimation::AddFramesDiagonally(const std::shared_ptr<SpriteSheet>& spriteSheet,
                                         unsigned int startXPos, unsigned int startYPos,
                                         unsigned int increment, unsigned int count)
    {
        unsigned int x = startXPos;
        unsigned int y = startYPos;
        for(unsigned int i = 0; i < startXPos + count; i += increment)
        {
            auto texCoords = spriteSheet->GetEvenSpriteTexCoords(x + i, y + i,
                                                                 0, 0, 0, 0);
            AddFrame(spriteSheet->GetTexture(), texCoords);
        }
    }

    void SpriteAnimation::IncrementFrameDelta(const Timestamp& delta,
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

    void SpriteAnimation::IncrementFrameDelta(double f, const Timestamp& max)
    {
        m_FrameDelta += (m_FrameDelta * f);
        if(m_FrameDelta > max) {
            m_FrameDelta = max;
        }
    }

    Sprite SpriteAnimation::GetActiveFrame() const
    {
        const SpriteFrame& frame = m_FrameQ.front();
        return { m_IDToTex.at(frame.id), frame.texCoords};
    }

    void SpriteAnimation::SetFrameDelta(const Timestamp& frameDelta)
    {
        m_FrameDelta = frameDelta;
    }
}

