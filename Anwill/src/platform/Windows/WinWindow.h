#pragma once

#include <memory>
#include <GLFW/glfw3.h>

#include "core/Window.h"
#include "gfx/GraphicsContext.h"

namespace Anwill {

    class WinWindow : public Window
    {
    public:
        WinWindow(const WindowSettings ws);

        void Update() override;
        void Terminate() override;
        unsigned int GetWidth() override;
        unsigned int GetHeight() override;


    private:
        void Init(const WindowSettings ws);

        std::unique_ptr<GraphicsContext> m_gc;
        GLFWwindow* m_Window;
        static bool s_Created;
    };

}