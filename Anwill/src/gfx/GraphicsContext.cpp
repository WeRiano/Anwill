#include "GraphicsContext.h"
#include "platform/OpenGL/OpenGLContext.h"
#include "Renderer.h"

namespace Anwill {

    std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch(Renderer::GetAPI())
        {
            case GraphicsAPI::API::OpenGL:
                return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }
        return nullptr;
    }
}