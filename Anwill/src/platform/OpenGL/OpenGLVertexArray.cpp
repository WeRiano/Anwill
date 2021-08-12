#include "platform/OpenGL/OpenGLVertexArray.h"
#include "core/Assert.h"

namespace Anwill {

    GLenum OpenGLVertexArray::ShaderDataTypeToOpenGLType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Float2:   return GL_FLOAT;
            case ShaderDataType::Float3:   return GL_FLOAT;
            case ShaderDataType::Float4:   return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
            default: AW_ASSERT(false, "Unknown ShaderDataType!"); return 0;
        }
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1, &m_ID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_ID);
    }

    void OpenGLVertexArray::AddBuffer(const VertexBuffer& buffer,
                                      const BufferLayout& layout)
    {
        const auto& elements = layout.GetElements();
        for (unsigned int i = 0; i < elements.size(); i++)
        {
            const auto& element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i,
                                  element.count,
                                  ShaderDataTypeToOpenGLType(element.type),
                                  element.normalized,
                                  layout.GetStride(), (const void*) element.offset);
        }
        /* TODO: Check for data type and call different glVertexAttribPointers?? Passing matrices
                 to the vertex shader?? (not by uniform) */
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_ID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
}