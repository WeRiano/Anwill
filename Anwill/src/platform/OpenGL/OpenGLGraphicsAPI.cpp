#include <glad.h>

#include "platform/OpenGL/OpenGLGraphicsAPI.h"
#include "gfx/ShaderMacros.h"
#include "core/Assert.h"

namespace Anwill {

    void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id,
                               unsigned severity, int length, const char* message,
                               const void* userParam)
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
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
                              0, NULL, GL_FALSE);
        AW_INFO("OpenGL debug output enabled!");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // This is probably going to bite me in the ass some day
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Texture slots is exclusive interval (we can go maximum 1 below when using glActiveTexture()
        int maxTextureSlots, maxFragTextureSlots, maxTextureLayers;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxFragTextureSlots);
        glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTextureLayers);
        AW_INFO("Maximum OpenGL texture slots: {0}", maxTextureSlots);
        AW_INFO("Maximum OpenGL fragment shader samplers: {0}", maxFragTextureSlots);
        AW_INFO("Maximum OpenGL texture layers: {0}", maxTextureLayers);
        ShaderMacros::SetMacro("AW_MAX_FRAGMENT_SAMPLERS", maxFragTextureSlots);

        // TODO: This stuff for 3D renderer.
        //glClearDepthf(0.0f);          // Depth Buffer Setup
        //glEnable(GL_DEPTH_TEST);      // Enables Depth Testing
        //glDepthFunc(GL_LESS);         // The Type Of Depth Testing To Do
    }

    void OpenGLGraphicsAPI::SetViewport(unsigned int x, unsigned int y,
                                        unsigned int width, unsigned int height) const
    {
        glViewport(x, y, width, height);
    }

    void OpenGLGraphicsAPI::SetClearColor(const Math::Vec3f& color) const
    {
        glClearColor(color.X, color.Y, color.Z, 1.0f);
    }

    void OpenGLGraphicsAPI::SetScissor(const Math::Vec2f& pos, const Math::Vec2f& size) const
    {
        glScissor(pos.X, pos.Y, size.X, size.Y);
    }

    void OpenGLGraphicsAPI::ClearBuffers() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLGraphicsAPI::DrawFont(Font& font, const std::string& text)
    {
        glDrawArrays(GL_TRIANGLES, 0, 6 * text.size());
        font.Done();
    }

    void OpenGLGraphicsAPI::DrawMesh(const Mesh &mesh)
    {
        mesh.Bind();
        glDrawElements(GL_TRIANGLES, mesh.GetIndexBufferCount(), GL_UNSIGNED_INT,
                       nullptr);
        mesh.Unbind();
    }

    void OpenGLGraphicsAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray,
                                        const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        vertexArray->Bind();
        indexBuffer->Bind();
        glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
        vertexArray->Unbind();
        indexBuffer->Unbind();
    }

    void OpenGLGraphicsAPI::DrawLine(const std::shared_ptr<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        glDrawArrays(GL_LINES, 0, 4);
        vertexArray->Unbind();
    }

    void OpenGLGraphicsAPI::DrawLines(const std::shared_ptr<VertexArray>& vertexArray,
                                      unsigned int vertexCount)
    {
        vertexArray->Bind();
        glDrawArrays(GL_LINE_STRIP, 0, vertexCount);
        vertexArray->Unbind();
    }

    std::shared_ptr<Shader> OpenGLGraphicsAPI::CreateQuadBatchShader() const
    {
        return Shader::Create("Anwill/res/shaders/OpenGL/AwQuadBatch.glsl");
    }

    std::shared_ptr<Shader> OpenGLGraphicsAPI::CreateCircleBatchShader() const
    {
        return Shader::Create("Anwill/res/shaders/OpenGL/AwCircleBatch.glsl");
    }
}