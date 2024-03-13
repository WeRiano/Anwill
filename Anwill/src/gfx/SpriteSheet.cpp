#include "gfx/SpriteSheet.h"
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

    Sprite SpriteSheet::GetSprite(unsigned int spriteXPos, unsigned int spriteYPos,
                                      int leftPadPixels, int rightPadPixels,
                                      int bottomPadPixels, int topPadPixels)
    {
        AW_PROFILE_FUNC();
        if(spriteXPos > m_SpriteCountX or spriteYPos > m_SpriteCountY) {
            AW_ERROR("Incorrect sprite coordinates.");
            return {nullptr, {-1.0f, -1.0f, -1.0f, -1.0f}};
        }
        QuadTexCoords texCoords = {
                Utils::NormalizeBetween0And1<int>(
                m_SpriteWidth * (spriteXPos - 1) - leftPadPixels,
                0,
                m_Texture->GetWidth()),
                Utils::NormalizeBetween0And1<int>(
                m_SpriteHeight * (spriteYPos - 1) - bottomPadPixels,
                0,
                m_Texture->GetHeight()),
                Utils::NormalizeBetween0And1<int>(
                m_SpriteWidth * spriteXPos + rightPadPixels,
                0,
                m_Texture->GetWidth()),
                Utils::NormalizeBetween0And1<int>(
                m_SpriteHeight * spriteYPos + topPadPixels,
                0,
                m_Texture->GetHeight())
        };

        return {m_Texture, texCoords};
    }

    Sprite SpriteSheet::GetSprite(unsigned int x, unsigned int y,
                                               unsigned int width, unsigned int height)
    {
        if((x + width) > m_Texture->GetWidth() or (y + height) > m_Texture->GetHeight()) {
            AW_ERROR("Incorrect sprite coordinates.");
            return {nullptr, {-1.0f, -1.0f, -1.0f, -1.0f}};
        }
        QuadTexCoords texCoords = { Utils::NormalizeBetween0And1(
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

        return {m_Texture, texCoords};
    }

    std::shared_ptr<Texture> SpriteSheet::GetTexture() const
    {
        return m_Texture;
    }
}