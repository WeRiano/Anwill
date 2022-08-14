#pragma once

#include "Texture.h"

namespace Anwill {

    /**
     * Wrapper class around Texture for additional spritesheet "operations"
     */
    class SpriteSheet
    {
    public:
        static std::shared_ptr<SpriteSheet> Create(const std::string& filePath,
                                                   const unsigned int spriteCountX,
                                                   const unsigned int spriteCountY);

        /**
         * @brief Get the tex coordinates of a sprite located in an even spritesheet
         * @param spriteXPos Sprite x position on the sheet. Starts at 1 from the left.
         * @param spriteYPos Sprite y position on the sheet. Starts at 1 from the bottom.
         * @param texX0 Resulting tex coordinate X0 position
         * @param texY0 Resulting tex coordinate Y0 position
         * @param texX1 Resulting tex coordinate X1 position
         * @param texY1 Resulting tex coordinate Y1 position
         */
        void GetEvenSpriteTexCoords(unsigned int spriteXPos, unsigned int spriteYPos,
                                    int pixelLeftPad, int pixelRightPad,
                                    int pixelBottomPad, int pixelTopPad,
                                    float& texX0, float& texY0,
                                    float& texX1, float& texY1);

        /**
         * @brief When you can't rely on the spritesheet having even sprites lined up
         *        and have to manually extract the texture coordinates of the sprite.
         * @param x Start X position in pixels from left
         * @param y Start Y position in pixels from bottom
         * @param width Sprite width in pixels
         * @param height Sprite height in pixels
         * @param texX0 Resulting tex coordinate X0 position
         * @param texY0 Resulting tex coordinate Y0 position
         * @param texX1 Resulting tex coordinate X1 position
         * @param texY1 Resulting tex coordinate Y1 position
         */
        void GetUnevenSpriteTexCoords(unsigned int x, unsigned int y,
                                      unsigned int width, unsigned int height,
                                      float& texX0, float& texY0,
                                      float& texX1, float& texY1);

        std::shared_ptr<Texture> GetTexture() const;
    private:
        std::shared_ptr<Texture> m_Texture;
        // Assuming an evenly distributed sprite sheet. See GetUnevenSpriteTexCoords if
        // this assumption is unrealistic
        unsigned int m_SpriteCountX, m_SpriteCountY, m_SpriteWidth, m_SpriteHeight;
    };
}
