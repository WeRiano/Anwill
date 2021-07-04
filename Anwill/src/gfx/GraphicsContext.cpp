#include "GraphicsContext.h"
#include "platform/OpenGL/OpenGLContext.h"
#include "Renderer.h"

namespace Anwill {

    std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch(Renderer::GetAPI())
        {
            case OpenGL:
                #ifdef AW_PLATFORM_WINDOWS
                return std::make_unique<OpenGLContext>(static_cast<HWND*>(window));
                #endif
        }
        return nullptr;
    }
}