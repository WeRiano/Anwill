#include "gfx/Shader.h"
#include "gfx/Renderer.h"
#include "core/Assert.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace Anwill {

    std::shared_ptr<Shader> Shader::Create(const std::string &filepath)
    {
        switch (Renderer::GetAPI())
        {
            case GraphicsAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(filepath);
            default:
                AW_ASSERT(false, "Shader does not support the chosen gfx API.");
                return nullptr;
        }
    }
}