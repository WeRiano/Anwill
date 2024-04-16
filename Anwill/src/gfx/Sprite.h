#pragma once

#include "gfx/Texture.h"
#include "gfx/Mesh.h"

namespace Anwill {

    struct QuadTexCoords
    {
        float x0, y0, x1, y1;
    };

    /**
     * A sprite consists of
     *     - a texture
     *     - a quad mesh with some texture coordinates (which are non-standard if texture is sheet)
     *
     * (but unique texture coordinates)
     */
    class Sprite
    {
    public:
        const std::shared_ptr<Texture> texture;
        QuadTexCoords texCoords;

        Sprite(std::shared_ptr<Texture> texture, const QuadTexCoords texCoords = {0.0f, 0.0f, 1.0f, 1.0f});

        /**
         * Used when a sprite wants to be rendered on its own
         * outside of batch renderer. Not preferred.
         */
        Mesh GetQuadMesh() const;
    };
}