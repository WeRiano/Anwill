#include "SpriteSheet.h"

#include "core/Log.h"
#include "utils/Utils.h"

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

    void
    SpriteSheet::GetEvenSpriteTexCoords(unsigned int spriteXPos, unsigned int spriteYPos,
                                        float& texX0, float& texY0,
                                        float& texX1, float& texY1)
    {
        if(spriteXPos > m_SpriteCountX or spriteYPos > m_SpriteCountY) {
            AW_ERROR("Incorrect sprite coordinates.");
            return;
        }
        texX0 = Utils::NormalizeBetween0And1<float>(
                static_cast<float>(m_SpriteWidth * (spriteXPos - 1)),
                0.0f,
                static_cast<float>(m_Texture->GetWidth()));
        texY0 = Utils::NormalizeBetween0And1<float>(
                static_cast<float>(m_SpriteHeight * (spriteYPos - 1)),
                0.0f,
                static_cast<float>(m_Texture->GetHeight()));
        texX1 = Utils::NormalizeBetween0And1<float>(
                static_cast<float>(m_SpriteWidth * spriteXPos),
                0.0f,
                static_cast<float>(m_Texture->GetWidth()));
        texY1 = Utils::NormalizeBetween0And1<float>(
                static_cast<float>(m_SpriteHeight * spriteYPos),
                0.0f,
                static_cast<float>(m_Texture->GetHeight()));
    }

    void SpriteSheet::GetUnevenSpriteTexCoords(unsigned int x, unsigned int y,
                                               unsigned int width, unsigned int height,
                                               float& texX0, float& texY0,
                                               float& texX1, float& texY1)
    {
        if((x + width) > m_Texture->GetWidth() or (y + height) > m_Texture->GetHeight()) {
            AW_ERROR("Incorrect sprite coordinates.");
            return;
        }
        texX0 = Utils::NormalizeBetween0And1<float>(
                static_cast<float>(x),
                0.0f,
                static_cast<float>(m_Texture->GetWidth()));
        texY0 = Utils::NormalizeBetween0And1<float>(
                static_cast<float>(y),
                0.0f,
                static_cast<float>(m_Texture->GetHeight()));
        texX1 = Utils::NormalizeBetween0And1<float>(
                static_cast<float>(x + width),
                0.0f,
                static_cast<float>(m_Texture->GetWidth()));
        texY1 = Utils::NormalizeBetween0And1<float>(
                static_cast<float>(y + height),
                0.0f,
                static_cast<float>(m_Texture->GetHeight()));
    }

    std::shared_ptr<Texture> SpriteSheet::GetTexture() const
    {
        return m_Texture;
    }
}