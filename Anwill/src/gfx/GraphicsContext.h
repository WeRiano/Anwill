#pragma once

#include <memory>

namespace Anwill {

    class GraphicsContext
    {
    public:
        virtual void SwapBuffers() = 0;
        virtual void Destroy() = 0;

        static std::unique_ptr<GraphicsContext> Create(void* window);
    };

}