#include "WinInput.h"

namespace Anwill {

    GLFWwindow* WinInput::s_Window = nullptr;

    void WinInput::Init(GLFWwindow* nativeWindow)
    {
        s_Window = nativeWindow;
    }

    bool WinInput::IsKeyPressed(const KeyCode& keyCode)
    {
        return glfwGetKey(s_Window, (int) keyCode) == GLFW_PRESS;
    }

    bool WinInput::IsMouseButtonDown(const MouseButtonCode& mouseButtonCode)
    {
        return glfwGetMouseButton(s_Window, (int) mouseButtonCode) == GLFW_PRESS;
    }
}