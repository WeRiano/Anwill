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
        : m_ScissorStack()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
                              0, NULL, GL_FALSE);
        AW_INFO("OpenGL debug output enabled!");

        // Blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Scissors
        glEnable(GL_SCISSOR_TEST);

        // This is probably going to bite me in the ass some day
        // No idea what it is
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
        glViewport((GLint) x, (GLint) y, (GLint) width, (GLint) height);
    }

    void OpenGLGraphicsAPI::SetScissor(const Math::Vec2f& pos, const Math::Vec2f& size) const
    {
        Scissor scissor{pos, size};
        scissor.Set();
    }

    void OpenGLGraphicsAPI::SetDefaultScissor() const
    {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glScissor(viewport[0], viewport[1], viewport[2], viewport[3]);
    }

    void OpenGLGraphicsAPI::PushScissor(const Math::Vec2f& pos, const Math::Vec2f& size)
    {
        m_ScissorStack.emplace(pos, size);
        Scissor scissor = m_ScissorStack.top();
        scissor.Set();
    }

    void OpenGLGraphicsAPI::EndScissor()
    {
        if(m_ScissorStack.empty()) { return; }
        // Pop the current scissor
        m_ScissorStack.pop();
        if(m_ScissorStack.empty())
        {
            // No more scissors on the stack, return to default
            SetDefaultScissor();
        } else
        {
            // Restore the scissor next in line
            Scissor nextScissor = m_ScissorStack.top();
            nextScissor.Set();
        }
    }

    void OpenGLGraphicsAPI::SetClearColor(const Math::Vec3f& color) const
    {
        glClearColor(color.X, color.Y, color.Z, 1.0f);
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

    void OpenGLGraphicsAPI::Scissor::Set() const
    {
        glScissor((GLint) pos.X, (GLint) pos.Y, (GLint) size.X, (GLint) size.Y);
    }
}