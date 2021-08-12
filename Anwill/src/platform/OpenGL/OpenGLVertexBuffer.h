#pragma once

#include "gfx/VertexBuffer.h"

namespace Anwill {

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(const float* data, unsigned int size);
        /***
         * Use for creation of a dynamic buffer (that will use DynamicUpdate())
         * @param size
         */
        OpenGLVertexBuffer(unsigned int size);
        ~OpenGLVertexBuffer();

        void DynamicUpdate(const float* data, unsigned int size) const;
        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_ID;
    };

}
