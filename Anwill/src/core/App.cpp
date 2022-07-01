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

    App::App(const WindowSettings& ws)
        : m_Running(true), m_Minimized(false)
    {
        m_Window = Window::Create(ws);

        Renderer::Init();

        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowClose);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowResize);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowFocus);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowMove);

    }

    void App::Run()
    {
        while(m_Running)
        {
            Renderer::ClearBuffers();

            m_LayerStack.Update();

            for(unsigned int i = 0; i < 2; i++)
            {
                SystemEvents::Pop();
            }

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