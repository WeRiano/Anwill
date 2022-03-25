#pragma once

#include <memory>

namespace Anwill {

    class IndexBuffer
    {
    public:
        static std::shared_ptr<IndexBuffer> Create(const unsigned int* indices,
                                                   const unsigned int count);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual unsigned int GetCount() const = 0;
    };

}