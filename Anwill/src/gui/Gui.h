#pragma once

#include <memory>
#include <imgui.h> // Include here so that client can use it

namespace Anwill {

    class Gui
    {
    public:
        static void Init(void* nativeWindow);
        static void Begin();
        static void Render();
        static void Terminate();
    };
}
