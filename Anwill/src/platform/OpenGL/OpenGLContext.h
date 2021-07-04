#pragma once

#include "gfx/GraphicsContext.h"
#include "core/PlatDef.h"

#ifdef AW_PLATFORM_WINDOWS
#include <Windows.h>
#endif
namespace Anwill {

    class OpenGLContext : public GraphicsContext
    {
    public:
        #ifdef AW_PLATFORM_WINDOWS
        OpenGLContext(HWND* hwnd);
        #endif

        void SwapBuffers();
        void Destroy();
    private:
        #ifdef AW_PLATFORM_WINDOWS
        HWND m_hWnd;
        HDC m_dc;       // Device context
        HGLRC m_rc;     // Rendering context
        #endif
    };
}
