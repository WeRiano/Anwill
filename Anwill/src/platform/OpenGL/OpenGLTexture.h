#pragma once

#include "gfx/Texture.h"

namespace Anwill {

    class OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture(const std::string& filePath);
        OpenGLTexture(const unsigned char* bitmapBuffer,
                      unsigned int bitmapWidth, unsigned int bitmapHeight);

        void Bind(unsigned int slot) const override;
        void Unbind() const override;

    private:
        unsigned int m_ID;
    };
}
