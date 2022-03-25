#include "App.h"

#include "core/Log.h"
#include "core/Window.h"
#include "gfx/Renderer.h"
#include "gfx/GraphicsContext.h"

// TODO: REMOVE, ONLY FOR TESTING
#include <glad.h>

#include "gfx/Shader.h"
#include "gfx/VertexBuffer.h"
#include "gfx/VertexArray.h"
#include "gfx/IndexBuffer.h"

#include "math/Mat4f.h"


namespace Anwill {

    App::App(const WindowSettings ws)
        : m_Running(true), m_Minimized(false)
    {
        m_Window = Window::Create(ws);

        Renderer::Init();

        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowClose);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowResize);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowFocus);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowMoved);

    }

    void App::Run()
    {
        /*
        float vertices[] = {

                500.0f, 500.0f,  // top right
                500.0f, 100.0f,  // bottom right
                100.0f, 100.0f,  // bottom left
                100.0f, 500.0f,  // top left
        };

        unsigned int indices[] = {
                0, 1, 3,
                1, 2, 3
        };

        auto vb = VertexBuffer::Create(vertices, sizeof(vertices));
        auto ib = IndexBuffer::Create(indices, 6);

        auto elements = {
                BufferElement(ShaderDataType::Float2)
        };

        auto bufferLayout = BufferLayout(elements);
        auto va = VertexArray::Create();
        va->AddBuffer(*vb.get(), bufferLayout);
        auto shader = Shader::Create("assets/shaders/HelloUniform.glsl");

        Mat4f ortho = Mat4f::Orthographic(0.0f, (float)m_Window->GetWidth(),
                                          0.0f, (float)m_Window->GetHeight(),
                                          -1.0f, 1.0f);
        //std::cout << ortho.ToString() << std::endl;

        shader->SetUniformMat4f(ortho, "u_ProjMat");

        shader->Bind();
        vb->Bind();
        va->Bind();
        ib->Bind();
        */

        while(m_Running)
        {
            m_LayerStack.Update();

            Renderer::ClearBuffers();

            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 6);

            SystemEvents::Pop();

            m_Window->Update();
        }
        m_Window->Terminate();
    }

    void App::OnEvent(std::unique_ptr<Event>& e)
    {
        EventHandler handler(*e);
        handler.Handle<WindowCloseEvent>(AW_BIND_EVENT_FN(OnWindowClose));
        handler.Handle<WindowResizeEvent>(AW_BIND_EVENT_FN(OnWindowResize));
        handler.Handle<WindowFocusEvent>(AW_BIND_EVENT_FN(OnWindowFocus));
        handler.Handle<WindowMoveEvent>(AW_BIND_EVENT_FN(OnWindowMove));
    }

    void App::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
    }

    void App::OnWindowResize(WindowResizeEvent& e)
    {
        AW_INFO("Resized Window to width {0} and height {1}.", e.GetNewWidth(), e.GetNewHeight());
        glViewport(0, 0, e.GetNewWidth(), e.GetNewHeight()); // TODO: Move to renderer or something
    }

    void App::OnWindowFocus(WindowFocusEvent e)
    {
        m_Minimized = !e.IsInFocus();
        if(m_Minimized) { AW_INFO("Application mininized."); }
        else { AW_INFO("Application in focus."); }
    }

    void App::OnWindowMove(WindowMoveEvent e)
    {
        //AW_INFO("Window moved to coordinates {0}, {1}", e.GetNewXPos(), e.GetNewYPos());
    }
}