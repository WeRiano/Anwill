#pragma once

#include <GLFW/glfw3.h>

#include "gfx/GraphicsContext.h"
#include "core/PlatDef.h"

namespace Anwill {

    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* window);
    };
}
