#include <utility>
#include <tchar.h>

#include "WinWindowOld.h"
#include "events/WindowEvents.h"
#include "events/SystemEventHandler.h"
#include "core/Log.h"

namespace Anwill {

    // WinWindow* WinWindow::s_Instance = nullptr; // TODO: Remove, keep?

    bool WinWindowOld::s_Created = false;

    WinWindowOld::WinWindowOld(const WindowSettings ws)
            : m_hInstance(GetModuleHandle(nullptr))
    {
        /*
        if (s_Instance == nullptr) // TODO: Remove, Keep?
        { */
        WindowInit(ws);
        while(!s_Created) {
            std::cout << "Waiting ... " << std::endl;
        }
        m_gc = GraphicsContext::Create(&m_hWnd);

        ShowWindow(m_hWnd, SW_SHOW);
        UpdateWindow(m_hWnd);
        //WinWindow::s_Instance = this;
        /* } else {
            AW_ERROR("Window instance already exists, cannot create two windows");
        }
         */
    }

    void WinWindowOld::Update()
    {
        UpdateWindow(m_hWnd);
        OnMsg();
    }

    void WinWindowOld::Terminate()
    {
        DestroyWindow(m_hWnd);
    }

    void WinWindowOld::ClearBuffers()
    {
        m_gc->ClearBuffers();
    }

    void WinWindowOld::SwapBuffers()
    {
        m_gc->SwapBuffers();
    }

    bool WinWindowOld::OnMsg()
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

    void WinWindowOld::WindowInit(const WindowSettings ws)
    {
        DWORD style = WS_OVERLAPPEDWINDOW; //WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE | ; // TODO: Play around with this

        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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
                rect.left,
                rect.top,
                rect.right - rect.left,
                rect.bottom - rect.top,
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
    }

    LRESULT WinWindowOld::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch(uMsg) {
            case WM_CREATE: {
                WinWindowOld::s_Created = true;
                break;
            }
            case WM_SETFOCUS: {
                WindowFocusEvent focus(true);
                SystemEvents::Add(focus);
                break;
            }
            case WM_KILLFOCUS: {
                WindowFocusEvent noFocus(false);
                SystemEvents::Add(noFocus);
                break;
            }
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
            case WM_PAINT:
                //std::cout << "HELLO!" << std::endl;
                // TODO: Wut is this
                break;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}
