#pragma once

#include "gfx/Sprite.h"

namespace Anwill {

    /**
     * Wrapper class around Texture for spritesheet operations.
     *
     * The intended workflow is:
     *      1. Create SpriteSheet
     *      2. Create Sprites from SpriteSheet
     *      (2.5 The SpriteSheet can now be discarded)
     *      3. Push Sprites to renderer
     */
    class SpriteSheet
    {
    public:
        static std::shared_ptr<SpriteSheet> Create(const std::string& filePath,
                                                   const unsigned int spriteCountX,
                                                   const unsigned int spriteCountY);

        /**
         * Get the tex coordinates of a sprite located in an even spritesheet.
         * @param spriteXPos Sprite x position on the sheet. Starts at 1 from the left.
         * @param spriteYPos Sprite y position on the sheet. Starts at 1 from the bottom.
         */

        /**
         * Get a @Sprite from an even spritesheet.
         * @param spriteXPos Sprite x position on the sheet. Starts at 1 from the left.
         * @param spriteYPos Sprite y position on the sheet. Starts at 1 from the bottom.
         * @param leftPadPixels Number of pixels to add to the left of the sprite.
         * @param rightPadPixels Number of pixels to add to the right of the sprite.
         * @param bottomPadPixels Number of pixels to add to the top of the sprite.
         * @param topPadPixels Number of pixels to add to the bottom of the sprite.
         * @return @Sprite object.
         */
        Sprite GetSprite(unsigned int spriteXPos,
                         unsigned int spriteYPos,
                         int leftPadPixels = 0, int rightPadPixels = 0,
                         int bottomPadPixels = 0, int topPadPixels = 0);

        /**
         * Get a @Sprite from an uneven spritesheet.
         * Used when you can't rely on the spritesheet having even sprites lined up
         * and have to manually extract the texture coordinates of the sprite.
         * @param x Start X position in pixels from left.
         * @param y Start Y position in pixels from bottom.
         * @param width Sprite width in pixels.
         * @param height Sprite height in pixels.
         */
        Sprite GetSprite(unsigned int x, unsigned int y,
                         unsigned int width, unsigned int height);

        std::shared_ptr<Texture> GetTexture() const;

    private:
        std::shared_ptr<Texture> m_Texture;
        // Assuming an evenly distributed sprite sheet. See GetUnevenSpriteTexCoords if
        // this assumption is unrealistic
        unsigned int m_SpriteCountX, m_SpriteCountY, m_SpriteWidth, m_SpriteHeight;
    };
}
