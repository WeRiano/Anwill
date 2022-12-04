#pragma once

#include <memory>
#include <GLFW/glfw3.h>

#include "core/Window.h"
#include "gfx/GraphicsContext.h"

namespace Anwill {

    class WinWindow : public Window
    {
    public:
        WinWindow(const WindowSettings& ws);

        void PreRenderUpdate() override;
        void PostRenderUpdate() override;
        void Terminate() override;
        void* GetNativeWindow() const override;
        unsigned int GetWidth() const override;
        unsigned int GetHeight() const override;


    private:
        void Init(const WindowSettings& ws);

        std::unique_ptr<GraphicsContext> m_gc;
        GLFWwindow* m_Window;
        static bool s_Created;
    };

}