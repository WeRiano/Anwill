#include "gfx/GraphicsContext.h"
#include "gfx/Renderer.h"
#include "gfx/ShaderMacros.h"
#include "gfx/Renderer2D.h"
#include "utils/Profiler.h"

#undef min

namespace Anwill {

    GraphicsAPI::API Renderer::s_APIName;
    std::shared_ptr<GraphicsAPI> Renderer::s_API = nullptr;

    void Renderer::Init()
    {
        s_API = GraphicsAPI::Create(s_APIName);

        Renderer2D::Init(s_API);
    }

    GraphicsAPI::API Renderer::GetAPI()
    {
        return s_APIName;
    }

    void Renderer::SetAPI(GraphicsAPI::API api)
    {
        s_APIName = api;
    }

    void Renderer::SetViewport(unsigned int x, unsigned int y,
                               unsigned int width, unsigned int height)
    {
        s_API->SetViewport(x, y, width, height);
    }

    void Renderer::SetScissor(const Math::Vec2f& pos, const Math::Vec2f& size)
    {
        s_API->SetScissor(pos, size);
    }

    void Renderer::ResetScissor()
    {
        s_API->ResetScissor();
    }

    void Renderer::SetClearColor(const Math::Vec3f& color)
    {
        s_API->SetClearColor(color);
    }

    void Renderer::ClearBuffers()
    {
        s_API->ClearBuffers();
    }
}