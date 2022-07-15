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
}