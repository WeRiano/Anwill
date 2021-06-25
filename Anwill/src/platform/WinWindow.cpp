#include <windows.h>
#include <tchar.h>

#include "WinWindow.h"
#include "core/Log.h"

namespace Anwill {

    WinWindow::WinWindow()
    {
        WNDCLASSEX wndClass = {};
        wndClass.lpszClassName = "WinWindow";
        wndClass.hInstance = GetModuleHandle(nullptr);
        wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

        if (!RegisterClassEx(&wndClass))
        {
            MessageBox(NULL,
                       _T("Call to RegisterClassEx failed!"),
                       _T("Windows Desktop Guided Tour"),
                       NULL);
            AW_ERROR("Windows Window registration failed");
        }
    }


}