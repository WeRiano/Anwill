#pragma once

#include <memory>
#include <GLFW/glfw3.h>

#include "core/KeyCodes.h"
#include "core/MouseCodes.h"

namespace Anwill {

    class WinInput
    {
    public:
        static void Init(GLFWwindow* nativeWindow);

        static bool IsKeyPressed(const KeyCode& keyCode);
        static bool IsMouseButtonDown(const MouseButtonCode& mouseButtonCode);

    private:
        static GLFWwindow* s_Window;
    };

}