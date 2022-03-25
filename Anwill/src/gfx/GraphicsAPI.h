#pragma once

#include <memory>

namespace Anwill {

    class GraphicsAPI
    {
    public:
        enum class API
        {
            OpenGL = 1
        };

        static std::unique_ptr<GraphicsAPI> Create(API api);

        virtual void ClearBuffers() = 0;
    };
}