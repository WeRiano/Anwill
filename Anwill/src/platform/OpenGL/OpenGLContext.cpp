#include <glad.h>

#include "OpenGLContext.h"
#include "core/Log.h"

namespace Anwill {

    OpenGLContext::OpenGLContext(void* window)
    {
        #ifdef AW_PLATFORM_WINDOWS
        auto glfwWindow = static_cast<GLFWwindow*>(window);
        glfwMakeContextCurrent(glfwWindow);
        glfwSwapInterval(0); // VSYNC off

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (!status) {
            AW_FATAL("GlAD failed to initialize!");
        } else {
            AW_INFO("Glad loaded! OpenGL version: {0}", (const char*) glGetString(GL_VERSION));
            int width, height;
            glfwGetWindowSize(glfwWindow, &width, &height);
            glViewport(0, 0, width, height);
        }
        #else
            #error OpenGLContext does not support this platform.
        #endif
    }


    /* DEPRECATED, SAVING FOR WHEN TO IMPLEMENT WIN32 API
    OpenGLContext::OpenGLContext(HWND* hwnd)
        : m_hWnd(*hwnd)
    {
        #ifdef AW_PLATFORM_WINDOWS
        HDC d_dc;
        HGLRC d_rc;

        WNDCLASSEX wc2;
        wc2.cbSize = sizeof(WNDCLASSEX);
        wc2.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
        wc2.cbClsExtra = 0;
        wc2.cbWndExtra = 0;
        wc2.hIcon = 0;
        wc2.hCursor = LoadCursor(0, IDC_ARROW);
        wc2.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
        wc2.lpszMenuName = 0;
        wc2.lpszClassName = TEXT("oglversionchecksample_class1");
        wc2.hIconSm = 0;

        if (!RegisterClassEx(&wc2)) {
            AW_FATAL("Failed to register dummy OpenGL window.");
        }

        HWND dummy_window = CreateWindowEx(
                0,
                wc2.lpszClassName,
                "Dummy OpenGL Window",
                0,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                WS_OVERLAPPEDWINDOW.hInstance,
                0);

        if (!dummy_window) {
            AW_FATAL("Failed to create dummy OpenGL window.");
        }

        PIXELFORMATDESCRIPTOR pfd = {
                sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
                1,                                          // Version Number
                PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
                PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
                PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
                PFD_TYPE_RGBA,                              // Request An RGBA Format
                32,                                       // Select Our Color Depth
                0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
                8,                                          // No Alpha Buffer
                0,                                          // Shift Bit Ignored
                0,                                          // No Accumulation Buffer
                0, 0, 0, 0,                                 // Accumulation Bits Ignored
                24,                                         // 16Bit Z-Buffer (Depth Buffer)
                8,                                          // No Stencil Buffer
                0,                                          // No Auxiliary Buffer
                PFD_MAIN_PLANE,                             // Main Drawing Layer
                0,                                          // Reserved
                0, 0, 0                                     // Layer Masks Ignored
        };
        d_dc = GetDC(dummy_window);
        int pf = ChoosePixelFormat(d_dc, &pfd);
        //DescribePixelFormat(m_dc, pf, sizeof(pfd), &pfd);
        SetPixelFormat(d_dc, pf, &pfd);
        d_rc = wglCreateContext(d_dc);
        if (!d_rc) {
            AW_ERROR("Failed to create dummy GL Rendering context");
        }
        if(!wglMakeCurrent(d_dc, d_rc)) {
            AW_ERROR("Failed to make dummy GL Rendering context current.");
        }

        int wglStatus = gladLoadWGL(d_dc);
        AW_INFO("Loading GLAD ...");
        if (!wglStatus)
        {
            AW_FATAL("WGL failed to load!");
        }

        wglMakeCurrent(d_dc, NULL);
        wglDeleteContext(d_rc);
        ReleaseDC(dummy_window, d_dc);
        DestroyWindow(dummy_window);

        int pixel_format_attribs[] = {
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB, 24,
                WGL_DEPTH_BITS_ARB, 24,
                WGL_STENCIL_BITS_ARB, 8,
                WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
                WGL_SAMPLES_ARB, 4,
                0
        };

        m_dc = GetDC(m_hWnd);

        int pixelFormat;
        UINT numFormats;
        wglChoosePixelFormatARB(m_dc, pixel_format_attribs, 0, 1, &pixelFormat, &numFormats);

        PIXELFORMATDESCRIPTOR new_pfd;
        DescribePixelFormat(m_dc, pixelFormat, sizeof(new_pfd), &new_pfd);
        if (!SetPixelFormat(m_dc, pixelFormat, &new_pfd)) {
            AW_FATAL("Failed to set the real opengl pixel format.");
        }

        int gl33_attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                WGL_CONTEXT_MINOR_VERSION_ARB, 6,
                WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0,
        };

        m_rc = wglCreateContextAttribsARB(m_dc, 0, gl33_attribs);
        if(!m_rc) {
            std::string errStr = GetLastErrorAsString();
            AW_FATAL("Failed to create real opengl context: {0}", errStr);
        }

        if (!wglMakeCurrent(m_dc, m_rc)) {
            AW_FATAL("Failed to activate real opengl rendering context.");
        }

        int glStatus = gladLoadGL();
        if (!glStatus)
        {
            AW_FATAL("Glad failed to load!");
        }
        RECT rect;
        GetWindowRect(*hwnd, &rect);
        glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top); // TODO: Move? Give window settings?
        AW_INFO("Glad loaded! OpenGL version: {0}", (const char*) glGetString(GL_VERSION));
        //AW_INFO("GL Extensions: {0}", (const char*) glGetString(GL_EXTENSIONS));
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
    */
}