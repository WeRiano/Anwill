#pragma once

#include <memory>

namespace Anwill {

    class VertexBuffer
    {
        static std::shared_ptr<VertexBuffer> Create(const float* vertices,
                                                    const unsigned int size);
        static std::shared_ptr<VertexBuffer> Create(const unsigned int size);
    };

}