#include "WinWindow.h"
#include "core/Log.h"
#include "core/KeyCodes.h"
#include "events/WindowEvents.h"
#include "events/SystemEventHandler.h"
#include "events/KeyEvents.h"
#include "events/MouseEvents.h"

namespace Anwill {

    bool WinWindow::s_Created = false;
    std::array<int, (std::size_t) SetMouseCursorEvent::CursorType::NumberOfCursorTypes>
    WinWindow::s_MouseCursorGLFWIDs = {
            GLFW_ARROW_CURSOR,
            GLFW_RESIZE_EW_CURSOR,
            GLFW_RESIZE_NS_CURSOR,
            GLFW_RESIZE_NESW_CURSOR,
            GLFW_RESIZE_NWSE_CURSOR,
            GLFW_IBEAM_CURSOR,
            GLFW_POINTING_HAND_CURSOR,
            GLFW_RESIZE_ALL_CURSOR
    };

    WinWindow::WinWindow(const WindowSettings& ws)
    {
        Init(ws);
    }

    void WinWindow::PreRenderUpdate()
    {
        glfwPollEvents();
    }

    void WinWindow::PostRenderUpdate()
    {
        glfwSwapBuffers(m_Window);
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
            SystemEventHandler::Add(resizeEvent);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowCloseEvent event;
            SystemEventHandler::Add(event);
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
            SystemEventHandler::Add(fEvent);
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowResizeEvent resizeEvent(width, height);
            SystemEventHandler::Add(resizeEvent);
        });

        glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos) {
            WindowMoveEvent moveEvent(xpos, ypos);
            SystemEventHandler::Add(moveEvent);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressEvent keyPressEvent((KeyCode) key);
                    SystemEventHandler::Add(keyPressEvent);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyRepeatEvent keyRepeatEvent((KeyCode) key);
                    SystemEventHandler::Add(keyRepeatEvent);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleaseEvent keyReleaseEvent((KeyCode) key);
                    SystemEventHandler::Add(keyReleaseEvent);
                    break;
                }
                default:
                {
                    AW_ERROR("Key action not recognized!");
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint)
        {
            KeyCharEvent keyCharEvent(static_cast<char>(codepoint));
            SystemEventHandler::Add(keyCharEvent);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
            int height;
            glfwGetWindowSize(window, nullptr, &height);

            MouseMoveEvent mouseMoveEvent((float) xpos, (float) height - (float) ypos);
            SystemEventHandler::Add(mouseMoveEvent);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods){
            switch(action) {
                case GLFW_PRESS:
                {
                    MouseButtonPressEvent mouseButtonPressEvent((MouseButtonCode) button);
                    SystemEventHandler::Add(mouseButtonPressEvent);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleaseEvent mouseButtonReleaseEvent((MouseButtonCode) button);
                    SystemEventHandler::Add(mouseButtonReleaseEvent);
                    break;
                }
                default:
                    AW_ERROR("Mouse button action recognized!");
                    break;
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset){
            if(xOffset)
            {
                MouseScrollEvent mouseScrollEvent((MouseScrollCode) xOffset);
                SystemEventHandler::Add(mouseScrollEvent);
            }
            else if(yOffset)
            {
                MouseScrollEvent mouseScrollEvent((MouseScrollCode) (yOffset + 1));
                SystemEventHandler::Add(mouseScrollEvent);
            }
        });

        SystemEventHandler::Subscribe<SetMouseCursorEvent>(
                AW_BIND_THIS_MEMBER_FUNC(WinWindow::OnSetCursorEvent));
    }

    void WinWindow::OnSetCursorEvent(const std::unique_ptr<Event>& event)
    {
        SetMouseCursorEvent e = static_cast<SetMouseCursorEvent&>(*event);
        auto cursor = glfwCreateStandardCursor(
                s_MouseCursorGLFWIDs[static_cast<int>(e.GetCursorType())]);
        glfwSetCursor(m_Window, cursor);
    }
}