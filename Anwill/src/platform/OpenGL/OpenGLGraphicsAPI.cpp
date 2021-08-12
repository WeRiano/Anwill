#include <glad.h>

#include "platform/OpenGL/OpenGLGraphicsAPI.h"
#include "core/Log.h"
#include "core/Assert.h"

namespace Anwill {

    void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length,
            const char* message, const void* userParam)
    {
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                AW_FATAL(message);
                return;
            case GL_DEBUG_SEVERITY_MEDIUM:
                AW_ERROR(message);
                return;
            case GL_DEBUG_SEVERITY_LOW:
                AW_WARN(message);
                return;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                AW_TRACE(message);
                return;
            default:
                AW_ASSERT(false, "OpenGL debug message severity unknown!");
                return;
        }
    }

    OpenGLGraphicsAPI::OpenGLGraphicsAPI()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);
        AW_INFO("OpenGL debug output enabled!");

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
    }
}