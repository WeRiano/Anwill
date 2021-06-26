#pragma once

#include <windows.h>
#include <tchar.h>

#include "core/Window.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace Anwill {

    class WinWindow : public Window
    {
    public:
        WinWindow(WindowSettings ws);

        bool OnMsg();

    private:
        HINSTANCE m_hInstance;  // Application
        HWND m_hWnd;            // Window
    };

}