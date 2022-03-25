#pragma once

#include <Windows.h>
#include <memory>

#include "core/Window.h"
#include "gfx/GraphicsContext.h"

namespace Anwill {

    class WinWindowOld : public Window
    {
    public:
        static bool s_Created;

        WinWindowOld(WindowSettings ws);

        void SwapBuffers() override;
        void ClearBuffers() override;
        void Update() override;
        void Terminate() override;

    private:
        bool OnMsg();
        void WindowInit(const WindowSettings ws);
        // In this function, you write code to handle messages that the application receives from Windows
        // when events occur.
        // For example, if a user chooses an OK button in your application,
        // Windows will send a message to you and you can write code inside your WndProc function that does
        // whatever work is appropriate.
        // It's called handling an event. You only handle the events that are relevant for your application.
        static LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        std::unique_ptr<GraphicsContext> m_gc;
        HINSTANCE m_hInstance;  // Application
        HWND m_hWnd;            // Window

        //static WinWindow* s_Instance; //TODO: Remove? Keep?
    };

}