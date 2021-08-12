#pragma once

#include <memory>

namespace Anwill {

    class IndexBuffer
    {
        static std::shared_ptr<IndexBuffer> Create(const unsigned int* indices,
                                                   const unsigned int count);
    };

}