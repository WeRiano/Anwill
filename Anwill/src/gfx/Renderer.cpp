#include "GraphicsContext.h"
#include "Renderer.h"

namespace Anwill {

    GraphicsAPI::API Renderer::s_APIName;
    std::unique_ptr<GraphicsAPI> Renderer::s_API;

    void Renderer::Init()
    {
        s_API = GraphicsAPI::Create(s_APIName);

        // TODO: Setup global graphics API settings (glBlend and stuff ... global settings)
    }

    void Renderer::SetAPI(GraphicsAPI::API api)
    {
        s_APIName = api;
    }

    GraphicsAPI::API Renderer::GetAPI()
    {
        return s_APIName;
    }

    void Renderer::ClearBuffers()
    {
        s_API->ClearBuffers();
    }
}