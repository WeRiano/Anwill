#pragma once

#include <imgui.h>

#include "core/Core.h"
#include "core/Layer.h"
#include "core/Window.h"

namespace Anwill {

    class Imgui
    {
    public:
        static void Init(const Window& window);
        static void NewFrame();
        static void EndFrame();
        static void Render();
    };
}
