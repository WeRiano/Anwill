#include "OpenGLContext.h"
#include "core/Log.h"
#include "OpenGLBase.h"

#include <string>
#include <format>

namespace Anwill {

    OpenGLContext::OpenGLContext(HWND* hwnd)
        : m_hWnd(*hwnd)
    {
        #ifdef AW_PLATFORM_WINDOWS
        PIXELFORMATDESCRIPTOR pfd = {
                sizeof(PIXELFORMATDESCRIPTOR),    //  size of this pfd
                1,                       // version number
                PFD_DRAW_TO_WINDOW |     // support window
                PFD_SUPPORT_OPENGL |             // support OpenGL
                PFD_DOUBLEBUFFER |                // double buffered
                PFD_TYPE_RGBA,                    // RGBA type
                32,                     // 24-bit color depth
                0, 0, 0, 0, 0, 0,      // color bits ignored
                8,                      // no alpha buffer
                0,                     // shift bit ignored
                0,                     // no accumulation buffer
                0, 0, 0, 0,            // accum bits ignored
                32,                     // 32-bit z-buffer
                8,                      // no stencil buffer
                0,                      // no auxiliary buffer
                PFD_MAIN_PLANE,                   // main layer
                0,                      // reserved
                0, 0, 0                // layer masks ignored
        };
        m_dc = GetDC(*hwnd);
        int pf = ChoosePixelFormat(m_dc, &pfd);
        SetPixelFormat(m_dc, pf, &pfd);
        m_rc = wglCreateContext(m_dc);
        wglMakeCurrent(m_dc, m_rc);
        int status = gladLoadGL();
        if (!status)
        {
            AW_FATAL("Glad failed to load!");
        }
        RECT rect;
        GetWindowRect(*hwnd, &rect);
        GLCall(glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top)); // TODO: Move? Give window settings?
        AW_INFO("OpenGL version: {0}", (const char*) glGetString(GL_VERSION));
        #endif
    }

    void OpenGLContext::SwapBuffers()
    {
        #ifdef AW_PLATFORM_WINDOWS
        ::SwapBuffers(m_dc);
        #endif
    }

    void OpenGLContext::Destroy()
    {
        ReleaseDC(m_hWnd, m_dc);
        wglDeleteContext(m_rc);
    }
}