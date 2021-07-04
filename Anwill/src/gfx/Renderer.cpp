#include "glad.h"

#include "GraphicsContext.h"
#include "Renderer.h"

namespace Anwill {

    GraphicsAPI Renderer::s_API;

    void Renderer::Init(GraphicsAPI api)
    {
        s_API = api;

        // TODO: Create graphics context
        // Setup global graphics API settings (glBlend for example ...)
    }

    GraphicsAPI Renderer::GetAPI()
    {
        return s_API;
    }
}