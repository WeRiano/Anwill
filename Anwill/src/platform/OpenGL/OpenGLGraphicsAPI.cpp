#include <glad.h>

#include "platform/OpenGL/OpenGLGraphicsAPI.h"
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

        // TODO: This stuff for 3D renderer.
        //glClearDepthf(0.01f);                                 // Depth Buffer Setup
        //glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
        //glDepthFunc(GL_LESS);                             // The Type Of Depth Testing To Do

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
    }

    void OpenGLGraphicsAPI::ClearBuffers()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // TODO: Make SetClearColor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}