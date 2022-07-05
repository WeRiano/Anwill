#pragma once

#include <glad.h>

#include "gfx/VertexArray.h"
#include "platform/OpenGL/OpenGLVertexBuffer.h"

namespace Anwill {

    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        virtual void AddBuffer(const VertexBuffer& buffer,
                               const BufferLayout& layout);
        void Bind() const;
        void Unbind() const;

        static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type);
    private:
        unsigned int m_ID;
    };

}