#include "gfx/VertexArray.h"
#include "gfx/Renderer.h"
#include "core/Assert.h"

#include "platform/OpenGL/OpenGLVertexArray.h"

namespace Anwill {

    BufferElement::BufferElement(ShaderDataType type, bool normalized)
        : type(type), size(GetSizeOfType(type)), offset(0),
          count(GetCountOfType(type)), normalized(normalized)
    {}

    unsigned int BufferElement::GetSizeOfType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:    return 4;
            case ShaderDataType::Float2:   return 4 * 2;
            case ShaderDataType::Float3:   return 4 * 3;
            case ShaderDataType::Float4:   return 4 * 4;
            case ShaderDataType::Mat3:     return 4 * 3 * 3;
            case ShaderDataType::Mat4:     return 4 * 4 * 4;
            case ShaderDataType::Int:      return 4;
            case ShaderDataType::Int2:     return 4 * 2;
            case ShaderDataType::Int3:     return 4 * 3;
            case ShaderDataType::Int4:     return 4 * 4;
            case ShaderDataType::Bool:     return 1;
            default: AW_ASSERT(false, "Size of ShaderDataType could not be determined.");
                     return 0;
        }
    }

    unsigned int BufferElement::GetCountOfType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:   return 1;
            case ShaderDataType::Float2:  return 2;
            case ShaderDataType::Float3:  return 3;
            case ShaderDataType::Float4:  return 4;
            case ShaderDataType::Mat3:    return 3; // 3* float3
            case ShaderDataType::Mat4:    return 4; // 4* float4
            case ShaderDataType::Int:     return 1;
            case ShaderDataType::Int2:    return 2;
            case ShaderDataType::Int3:    return 3;
            case ShaderDataType::Int4:    return 4;
            case ShaderDataType::Bool:    return 1;
            default: AW_ASSERT(false, "Count of ShaderDataType could not be determined.");
                     return 0;
        }
    }

    BufferLayout::BufferLayout(std::vector<BufferElement>&& elements)
    {
        m_Elements = std::move(elements);
        CalcStrideAndOffset();
    }

    BufferLayout::BufferLayout(std::vector<BufferElement> elements)
    {
        m_Elements = std::move(elements);
        CalcStrideAndOffset();
    }

    const std::vector<BufferElement> BufferLayout::GetElements() const
    {
        return m_Elements;
    }

    const unsigned int BufferLayout::GetStride() const
    {
        return m_Stride;
    }

    void BufferLayout::CalcStrideAndOffset()
    {
        unsigned int offset = 0;
        m_Stride = 0;
        for (auto& element : m_Elements)
        {
            element.offset = offset;
            m_Stride += element.size;

            offset += element.size;
        }
    }

    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        switch(Renderer::GetAPI())
        {
            case GraphicsAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexArray>();
            default:
                AW_ASSERT(false, "VertexArray does not support the chosen gfx API.");
                return nullptr;
        }
    }

}