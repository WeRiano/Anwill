#include "WinWindow.h"
#include "core/Log.h"
#include "core/KeyCodes.h"
#include "events/WindowEvents.h"
#include "events/SystemEvents.h"
#include "events/KeyEvents.h"
#include "events/MouseEvents.h"

namespace Anwill {

    bool WinWindow::s_Created = false;

    WinWindow::WinWindow(const WindowSettings& ws)
    {
        Init(ws);
    }

    void WinWindow::Update()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void WinWindow::Terminate()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void* WinWindow::GetNativeWindow() const
    {
        return m_Window;
    }

    unsigned int WinWindow::GetWidth() const
    {
        int width;
        glfwGetWindowSize(m_Window, &width, nullptr);
        return width;
    }

    unsigned int WinWindow::GetHeight() const
    {
        int height;
        glfwGetWindowSize(m_Window, nullptr, &height);
        return height;
    }


    void WinWindow::Init(const WindowSettings& ws)
    {
        if (s_Created)
        {
            AW_ERROR("A window already exists, can't create another one.");
        }

        if (!glfwInit())
        {
            AW_FATAL("GLFW failed to initialize!");
            return;
        }
        AW_INFO("GLFW initialized successfully!");

        glfwSetErrorCallback([](int error, const char* description) {
            AW_ERROR("A GLFW error occurred! Error code: {0}. Description: {1}", error, description);
        });

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // TODO: Let user set opengl version (?)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(ws.width, ws.height, ws.title, nullptr, nullptr);

        if (!m_Window)
        {
            AW_FATAL("GLFW window or opengl context creation failed.");
            glfwTerminate();
            return;
        }

        s_Created = true;

        m_gc = GraphicsContext::Create(m_Window);

        // Define and setting window callbacks
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowResizeEvent resizeEvent(width, height);
            SystemEvents::Add(resizeEvent);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowCloseEvent event;
            SystemEvents::Add(event);
        });
        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
            bool focus;
            switch (focused)
            {
                case GLFW_TRUE:
                    focus = true;
                    break;
                case GLFW_FALSE:
                    focus = false;
                    break;
                default:
                    AW_ERROR("unrecognizable 'focused' variable in glfwWindowFocusCallback.");
                    return;
            }
            WindowFocusEvent fEvent(focus);
            SystemEvents::Add(fEvent);
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowResizeEvent resizeEvent(width, height);
            SystemEvents::Add(resizeEvent);
        });

        glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos) {
            WindowMoveEvent moveEvent(xpos, ypos);
            SystemEvents::Add(moveEvent);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressEvent keyPressEvent((Anwill::KeyCode) key);
                    SystemEvents::Add(keyPressEvent);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyRepeatEvent keyRepeatEvent((Anwill::KeyCode) key);
                    SystemEvents::Add(keyRepeatEvent);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleaseEvent keyReleaseEvent((Anwill::KeyCode) key);
                    SystemEvents::Add(keyReleaseEvent);
                    break;
                }
                default:
                {
                    AW_ERROR("Key action not recognized!");
                    break;
                }
            }
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
            int height;
            glfwGetWindowSize(window, nullptr, &height);

            MouseMoveEvent mouseMoveEvent((float) xpos, (float) height - (float) ypos);
            SystemEvents::Add(mouseMoveEvent);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods){
            switch(action) {
                case GLFW_PRESS:
                {
                    MouseButtonPressEvent mouseButtonPressEvent((Anwill::MouseButtonCode) button);
                    SystemEvents::Add(mouseButtonPressEvent);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleaseEvent mouseButtonReleaseEvent((Anwill::MouseButtonCode) button);
                    SystemEvents::Add(mouseButtonReleaseEvent);
                    break;
                }
                default:
                    AW_ERROR("Mouse button action recognized!");
                    break;
            }
        });
    }
}