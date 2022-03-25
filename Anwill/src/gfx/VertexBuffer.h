#pragma once

#include <memory>

namespace Anwill {

    class VertexBuffer
    {
    public:
        static std::shared_ptr<VertexBuffer> Create(const float* vertices,
                                                    const unsigned int size);
        static std::shared_ptr<VertexBuffer> Create(const unsigned int size);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };

}