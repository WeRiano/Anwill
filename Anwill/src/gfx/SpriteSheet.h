#pragma once

#include "Texture.h"

namespace Anwill {

    struct QuadTexCoords {
        float x0, y0, x1, y1;
    };

    /**
     * Wrapper class around Texture for additional spritesheet "operations"
     *
     * The intended workflow is:
     *      1. Create SpriteSheet
     *      2. Create Sprites from SpriteSheet
     *      (2.5) The SpriteSheet can now be discarded
     *      3. Push Sprites to renderer
     *
     * PRO: Exists so that we don't have to calculate(?) texCoords every cycle for the batch renderer,
     *       even though this is a really quick O(1) calculation.
     * CON: 16 bytes of memory for each Sprite.
     *      FIX: Static map with all the textures, and Sprites carry an id instead?
     *           (4 bytes vs 16)
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
         */
        QuadTexCoords GetEvenSpriteTexCoords(unsigned int spriteXPos,
                                             unsigned int spriteYPos,
                                             int pixelLeftPad, int pixelRightPad,
                                             int pixelBottomPad, int pixelTopPad);

        /**
         * @brief When you can't rely on the spritesheet having even sprites lined up
         *        and have to manually extract the texture coordinates of the sprite.
         * @param x Start X position in pixels from left
         * @param y Start Y position in pixels from bottom
         * @param width Sprite width in pixels
         * @param height Sprite height in pixels
         */
        QuadTexCoords GetUnevenSpriteTexCoords(unsigned int x, unsigned int y,
                                               unsigned int width, unsigned int height);

        std::shared_ptr<Texture> GetTexture() const;

    private:
        std::shared_ptr<Texture> m_Texture;
        // Assuming an evenly distributed sprite sheet. See GetUnevenSpriteTexCoords if
        // this assumption is unrealistic
        unsigned int m_SpriteCountX, m_SpriteCountY, m_SpriteWidth, m_SpriteHeight;
    };

    /**
     * A sprite is just a texture combined with a square mesh and standard texture coordinates.
     * This exists to simplify the process of rendering a square texture, both internally and by the client
     */
    struct Sprite
    {
        const std::shared_ptr<Texture> texture;
        QuadTexCoords texCoords;

        Sprite(std::shared_ptr<Texture> texture,
               const QuadTexCoords texCoords = {0.0f, 0.0f, 1.0f, 1.0f})
               : texture(std::move(texture)), texCoords(texCoords)
        {}

        Sprite(const std::shared_ptr<SpriteSheet>& spriteSheet,
               unsigned int spriteSheetXPos, unsigned int spriteSheetYPos,
               int pixelLeftPad = 0, int pixelRightPad = 0,
               int pixelBottomPad = 0, int pixelTopPad = 0)
               : texture(spriteSheet->GetTexture()),
                 texCoords(spriteSheet->GetEvenSpriteTexCoords(spriteSheetXPos,
                                                               spriteSheetYPos,
                                                               pixelLeftPad,
                                                               pixelRightPad,
                                                               pixelBottomPad,
                                                               pixelTopPad))
       {}
    };
}
