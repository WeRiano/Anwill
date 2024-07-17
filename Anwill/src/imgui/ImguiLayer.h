#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "core/Core.h"
#include "core/Layer.h"
#include "core/Window.h"

namespace Anwill {

    class ImguiLayer : public Layer
    {
    public:
        static void Init(const Window& window);

        explicit ImguiLayer(unsigned int ups, const Window& window);

        void Update(const Timestamp& timestamp) override;
    };
}
