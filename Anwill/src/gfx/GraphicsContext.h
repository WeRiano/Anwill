#pragma once

#include <memory>

// This is probably useful when creating a context becomes super difficult (hello win32 api) but since it is so trivial
// for glfw / opengl lets keep class spam down :)
namespace Anwill {

    class GraphicsContext
    {
    public:
        static std::unique_ptr<GraphicsContext> Create(void* window);
    };

}