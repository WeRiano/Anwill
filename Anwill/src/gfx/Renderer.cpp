#include "glad.h"

#include "GraphicsContext.h"
#include "Renderer.h"

namespace Anwill {

    GraphicsAPI Renderer::s_API;

    void Renderer::Init(GraphicsAPI api)
    {
        s_API = api;

        // TODO: Setup global graphics API settings (glBlend and stuff ... global settings)
    }

    GraphicsAPI Renderer::GetAPI()
    {
        return s_API;
    }
}