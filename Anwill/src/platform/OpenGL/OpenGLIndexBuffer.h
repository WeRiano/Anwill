#pragma once

#include <glad.h>

#include "gfx/IndexBuffer.h"

namespace Anwill {

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(const unsigned int* indices, unsigned int count);
        OpenGLIndexBuffer(unsigned int count);
        ~OpenGLIndexBuffer();

        void DynamicUpdate(const void* data, unsigned int size) override;
        void Bind() const;
        void Unbind() const;
        unsigned int GetCount() const;

    private:
        unsigned int m_ID;
        unsigned int m_Count;
    };

}