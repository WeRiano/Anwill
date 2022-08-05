#include "Texture.h"
#include "gfx/Renderer.h"
#include "platform/OpenGL/OpenGLTexture.h"
#include "core/Assert.h"

namespace Anwill {

    std::shared_ptr<Texture> Texture::Create(const std::string& filePath)
    {
        switch (Renderer::GetAPI())
        {
            case GraphicsAPI::API::OpenGL:
                return std::make_shared<OpenGLTexture>(filePath);
            default:
                AW_ASSERT(false, "Texture does not support the chosen gfx API.");
                return nullptr;
        }
    }

    std::shared_ptr<Texture> Texture::Create(const unsigned char* bitmapBuffer,
                                             unsigned int bitmapWidth,
                                             unsigned int bitmapHeight)
    {
        switch (Renderer::GetAPI())
        {
            case GraphicsAPI::API::OpenGL:
                return std::make_shared<OpenGLTexture>(bitmapBuffer, bitmapWidth,
                                                       bitmapHeight);
            default:
                AW_ASSERT(false, "Texture does not support the chosen gfx API.");
                return nullptr;
        }
    }

    unsigned int Texture::GetWidth() const
    {
        return m_Width;
    }

    unsigned int Texture::GetHeight() const
    {
        return m_Height;
    }
}