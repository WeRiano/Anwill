#include "SpriteSheet.h"

#include "core/Log.h"
#include "utils/Utils.h"
#include "utils/Profiler.h"

namespace Anwill {
    std::shared_ptr<SpriteSheet> SpriteSheet::Create(const std::string& filePath,
                                                     const unsigned int spriteCountX,
                                                     const unsigned int spriteCountY)
    {
        std::shared_ptr<SpriteSheet> spriteSheet = std::make_shared<SpriteSheet>();
        spriteSheet->m_Texture = Texture::Create(filePath);
        spriteSheet->m_SpriteCountX = spriteCountX;
        spriteSheet->m_SpriteCountY = spriteCountY;
        spriteSheet->m_SpriteWidth = spriteSheet->m_Texture->GetWidth() /
                spriteSheet->m_SpriteCountX;
        spriteSheet->m_SpriteHeight = spriteSheet->m_Texture->GetHeight() /
                spriteSheet->m_SpriteCountY;
        return spriteSheet;
    }

    QuadTexCoords
    SpriteSheet::GetEvenSpriteTexCoords(unsigned int spriteXPos, unsigned int spriteYPos,
                                        int pixelLeftPad, int pixelRightPad,
                                        int pixelBottomPad, int pixelTopPad)
    {
        AW_PROFILE_FUNC();
        if(spriteXPos > m_SpriteCountX or spriteYPos > m_SpriteCountY) {
            AW_ERROR("Incorrect sprite coordinates.");
            return {-1.0f, -1.0f, -1.0f, -1.0f};
        }
        return { Utils::NormalizeBetween0And1<int>(
                m_SpriteWidth * (spriteXPos - 1) - pixelLeftPad,
                0,
                m_Texture->GetWidth()),
                Utils::NormalizeBetween0And1<int>(
                m_SpriteHeight * (spriteYPos - 1) - pixelBottomPad,
                0,
                m_Texture->GetHeight()),
                Utils::NormalizeBetween0And1<int>(
                m_SpriteWidth * spriteXPos + pixelRightPad,
                0,
                m_Texture->GetWidth()),
                Utils::NormalizeBetween0And1<int>(
                m_SpriteHeight * spriteYPos + pixelTopPad,
                0,
                m_Texture->GetHeight()) };
    }

    QuadTexCoords SpriteSheet::GetUnevenSpriteTexCoords(unsigned int x, unsigned int y,
                                               unsigned int width, unsigned int height)
    {
        if((x + width) > m_Texture->GetWidth() or (y + height) > m_Texture->GetHeight()) {
            AW_ERROR("Incorrect sprite coordinates.");
            return {-1.0f, -1.0f, -1.0f, -1.0f};
        }
        return { Utils::NormalizeBetween0And1(
                x,
                0u,
                m_Texture->GetWidth()),
                Utils::NormalizeBetween0And1(
                y,
                0u,
                m_Texture->GetHeight()),
                Utils::NormalizeBetween0And1(
                x + width,
                0u,
                m_Texture->GetWidth()),
                Utils::NormalizeBetween0And1(
                y + height,
                0u,
                m_Texture->GetHeight()) };
    }

    std::shared_ptr<Texture> SpriteSheet::GetTexture() const
    {
        return m_Texture;
    }
}