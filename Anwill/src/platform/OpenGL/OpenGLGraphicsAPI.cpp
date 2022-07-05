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
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
        AW_INFO("OpenGL debug output enabled!");

        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // TODO: This stuff for 3D renderer.
        //glClearDepthf(0.0f);                                 // Depth Buffer Setup
        //glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
        //glDepthFunc(GL_LESS);                             // The Type Of Depth Testing To Do
    }

    void OpenGLGraphicsAPI::SetViewport(float x, float y, float width, float height) const
    {
        glViewport(x, y, width, height);
    }

    void OpenGLGraphicsAPI::SetClearColor(const Math::Vec3f& color) const
    {
        glClearColor(color.GetX(), color.GetY(), color.GetZ(), 1.0f);
    }

    void OpenGLGraphicsAPI::ClearBuffers() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLGraphicsAPI::Draw(const Mesh &mesh)
    {
        mesh.Bind();
        glDrawElements(GL_TRIANGLES, mesh.GetIndexBufferCount(), GL_UNSIGNED_INT, nullptr);
        mesh.Unbind();
    }

    void OpenGLGraphicsAPI::Draw(const std::shared_ptr<VertexArray>& vertexArray,
                                 const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        vertexArray->Bind();
        indexBuffer->Bind();
        glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
        vertexArray->Unbind();
        indexBuffer->Unbind();
    }
}