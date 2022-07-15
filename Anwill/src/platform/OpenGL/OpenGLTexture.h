#pragma once

#include "gfx/Texture.h"

namespace Anwill {

    class OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture(const std::string& filePath);

        void Bind() const override;
        void Unbind() const override;

    private:
        unsigned int m_ID;
    };
}
