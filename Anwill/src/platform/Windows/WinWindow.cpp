#include <utility>
#include <tchar.h>

#include "WinWindow.h"
#include "events/WindowEvents.h"
#include "events/SystemEvents.h"
#include "core/Log.h"

namespace Anwill {

    // WinWindow* WinWindow::s_Instance = nullptr; // TODO: Remove, keep?

    WinWindow::WinWindow(const WindowSettings ws)
            : m_hInstance(GetModuleHandle(nullptr))
    {
        /*
        if (s_Instance == nullptr) // TODO: Remove, Keep?
        { */
        WindowInit(ws);
        m_gc = GraphicsContext::Create(&m_hWnd);
        //WinWindow::s_Instance = this;
        /* } else {
            AW_ERROR("Window instance already exists, cannot create two windows");
        }
         */
    }

    void WinWindow::Update()
    {
        UpdateWindow(m_hWnd);
        OnMsg();
        m_gc->SwapBuffers();
    }

    void WinWindow::Terminate()
    {
        DestroyWindow(m_hWnd);
    }

    bool WinWindow::OnMsg()
    {
        MSG msg = {};
        // Peek instead of Get, which doesnt wait for one if there isnt one (it just checks and continues)
        // PM_Remove = Remove msg after peeking
        while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return false;
            }
            TranslateMessage(&msg); // Translating key presses into characters and stuff
            DispatchMessage(&msg); // This causes the WindowProc
        }
        return true;
    }

    void WinWindow::WindowInit(const WindowSettings ws)
    {
        DWORD style = WS_VISIBLE | WS_OVERLAPPEDWINDOW; //WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU; // TODO: Play around with this

        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WindowProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = m_hInstance;
        wcex.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(m_hInstance, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH) (COLOR_BACKGROUND);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = "Class Name";
        wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

        if (!RegisterClassEx(&wcex)) {
            MessageBox(NULL,
                       _T("Call to RegisterClassEx failed!"),
                       _T("Windows Desktop Guided Tour"), // Pops out a window that freezes the app
                       NULL);
            unsigned int errorCode = GetLastError();
            AW_ERROR("Windows Window registration failed. Error code: {0}", std::to_string(errorCode));
        }

        RECT rect;
        rect.left = 150;
        rect.top = 70;
        rect.right = rect.left + ws.width;
        rect.bottom = rect.top + ws.height;

        // window size is set to the canvas board (excluding the border).
        //AdjustWindowRect(&rect, style, false);

        m_hWnd = CreateWindow(
                "Class Name", // TODO: Should probably be ws.name or something
                "Window Title",
                style,
                0,//rect.left,
                0,//rect.top,
                ws.width,//rect.right - rect.left,
                ws.height,//rect.bottom - rect.top,
                NULL, // Something to do with menus?
                NULL,
                m_hInstance,
                NULL
        );
        if (!m_hWnd) {
            MessageBox(NULL,
                       _T("Call to CreateWindow failed!"),
                       _T("Windows Desktop Guided Tour"),
                       NULL);
            AW_ERROR("Window object failed to initialize.");
        }

        ShowWindow(m_hWnd, SW_SHOW);
        UpdateWindow(m_hWnd);
    }

    LRESULT WinWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch(uMsg) {
            case WM_SIZE: {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                WindowResizeEvent event(width, height);
                SystemEvents::Add(event);
                break;
            }
            case WM_CLOSE: {
                WindowCloseEvent event;
                SystemEvents::Add(event);
                break;
            }
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}
