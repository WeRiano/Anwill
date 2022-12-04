#pragma once

#include <memory>


namespace Anwill {

    class GraphicsContext
    {
    public:
        static std::unique_ptr<GraphicsContext> Create(void* nativeWindow);
    };

}