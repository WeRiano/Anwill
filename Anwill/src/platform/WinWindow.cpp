#include "WinWindow.h"
#include "core/Log.h"

// In this function, you write code to handle messages that the application receives from Windows when events occur.
// For example, if a user chooses an OK button in your application,
// Windows will send a message to you and you can write code inside your WndProc function that does whatever work is appropriate.
// It's called handling an event. You only handle the events that are relevant for your application.
LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY: // When the window is destroyed after being closed ... (se prev case).
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

namespace Anwill {

    WinWindow::WinWindow(const WindowSettings ws)
        : m_hInstance(GetModuleHandle(nullptr))
    {
        WNDCLASSEX wndClass = {};
        wndClass.lpszClassName = "Class Name"; // TODO: Should probably be ws.name or something
        wndClass.hInstance = GetModuleHandle(nullptr);
        wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // TODO: Play around with cursor and icon! Swap em out to something custom
        wndClass.lpfnWndProc = WindowProc;

        if (!RegisterClassEx(&wndClass))
        {
            MessageBox(NULL,
                       _T("Call to RegisterClassEx failed!"),
                       _T("Windows Desktop Guided Tour"), // Pops out a window that freezes the app
                       NULL);
            AW_ERROR("Windows Window registration failed");
        }

        DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU; // TODO: Play around with this

        RECT rect;
        rect.left = 250;
        rect.top = 250;
        rect.right = rect.left + ws.width;
        rect.bottom = rect.top + ws.height;

        AdjustWindowRect(&rect, style, false); //window size is set to the canvas board (excluding the border).

        m_hWnd = CreateWindowEx(
                0,
                "Class Name", // TODO: Should probably be ws.name or something
                "Window Title",
                style,
                rect.left,
                rect.top,
                rect.right - rect.left,
                rect.bottom - rect.top,
                NULL, // Something to do with menus?
                NULL,
                m_hInstance,
                NULL
                );

        ShowWindow(m_hWnd, SW_SHOW);
    }

    bool WinWindow::OnMsg()
    {
        MSG msg = {};

        // Peek instead of Get, which doesnt wait for one if there isnt one (it just checks and continues)
        // PM_Remove = Remove msg after peeking
        while(PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return false;
            }

            TranslateMessage(&msg); // Translating key presses into characters and stuff
            DispatchMessage(&msg); // This causes the WindowProc
        }

        return true;
    }

}