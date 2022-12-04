#include "VertexBuffer.h"

#include "core/Assert.h"
#include "gfx/Renderer.h"
#include "platform/OpenGL/OpenGLVertexBuffer.h"

namespace Anwill {

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(const float* vertices,
                                                       const unsigned int size)
    {
        switch(Renderer::GetAPI())
        {
            case GraphicsAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexBuffer>(vertices, size);
            default:
                AW_ASSERT(false, "Vertex buffer does not support the chosen graphics "
                                 "API.");
                return nullptr;
        }
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(const unsigned int size)
    {
        switch(Renderer::GetAPI())
        {
            case GraphicsAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexBuffer>(size);
            default:
                AW_ASSERT(false, "Vertex buffer does not support the chosen gfx API.");
                return nullptr;
        }
    }
}