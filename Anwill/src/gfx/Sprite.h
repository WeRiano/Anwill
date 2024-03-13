#pragma once

#include "gfx/Texture.h"

namespace Anwill {

    struct QuadTexCoords
    {
        float x0, y0, x1, y1;
    };

    /**
     * A sprite consists of
     *     - a rectangular mesh
     *     - a texture with some texture coordinates
     * This exists to simplify the process of rendering a quad with a shared texture
     * (but unique texture coordinates)
     */
    struct Sprite
    {
        const std::shared_ptr<Texture> texture;

        QuadTexCoords texCoords;

        Sprite(std::shared_ptr<Texture> texture,
               const QuadTexCoords texCoords = {0.0f, 0.0f, 1.0f, 1.0f})
                : texture(std::move(texture)), texCoords(texCoords) {}

        // DEPRECATED - we swap the dependencies (sheet looks at sprite instead)
        /* Sprite(const std::shared_ptr<SpriteSheet>& spriteSheet,
               unsigned int spriteSheetXPos, unsigned int spriteSheetYPos,
               int pixelLeftPad = 0, int pixelRightPad = 0,
               int pixelBottomPad = 0, int pixelTopPad = 0); */
    };
}