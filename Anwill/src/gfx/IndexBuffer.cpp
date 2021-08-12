#include "gfx/IndexBuffer.h"
#include "core/Assert.h"
#include "gfx/Renderer.h"
#include "platform/OpenGL/OpenGLIndexBuffer.h"

namespace Anwill {

    std::shared_ptr<IndexBuffer> IndexBuffer::Create(const unsigned int* indices,
                                                     const unsigned int count)
    {
        switch(Renderer::GetAPI())
        {
            case GraphicsAPI::API::OpenGL:
                return std::make_shared<OpenGLIndexBuffer>(indices, count);
            default:
                AW_ASSERT(false, "Index buffer does not support the chosen gfx API.");
                return nullptr;
        }
    }
}