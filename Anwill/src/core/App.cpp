#include "App.h"

#include "core/Log.h"
#include "core/Window.h"
#include "core/Input.h"

#include "gfx/Renderer.h"
#include "gfx/GraphicsContext.h"
#include "gfx/Shader.h"
#include "gfx/VertexBuffer.h"
#include "gfx/VertexArray.h"
#include "gfx/IndexBuffer.h"

#include "ecs/Ecs.h"

#include "utils/Random.h"


namespace Anwill {

    App::App(const WindowSettings& ws)
        : m_Running(true), m_Minimized(false)
    {
        m_Window = Window::Create(ws);

        Input::Init(m_Window->GetNativeWindow());
        Renderer::Init();
        Ecs::Init();
        Random::Init();

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

            // TODO: Dynamic event popping depending on q size and/or growth/decay
            for(unsigned int i = 0; i < 2; i++)
            {
                SystemEvents::Pop();
            }

            m_Window->Update();
        }
        m_Window->Terminate();
    }

    bool App::IsMinimized() const
    {
        return m_Minimized;
    }

    void App::OnEvent(std::unique_ptr<Event>& e)
    {
        EventHandler handler(*e);
        handler.Handle<WindowCloseEvent>(AW_BIND_EVENT_FN(OnWindowClose));
        handler.Handle<WindowResizeEvent>(AW_BIND_EVENT_FN(OnWindowResize));
        handler.Handle<WindowFocusEvent>(AW_BIND_EVENT_FN(OnWindowFocus));
        handler.Handle<WindowMoveEvent>(AW_BIND_EVENT_FN(OnWindowMove));
    }

    void App::OnWindowClose(const WindowCloseEvent& e)
    {
        m_Running = false;
    }

    void App::OnWindowResize(const WindowResizeEvent& e)
    {
        AW_INFO("Resized Window to width {0} and height {1}.", e.GetNewWidth(), e.GetNewHeight());
        Renderer::SetViewport(0, 0, e.GetNewWidth(), e.GetNewHeight());
    }

    void App::OnWindowFocus(const WindowFocusEvent& e)
    {
        m_Minimized = !e.IsInFocus();
        if(m_Minimized) { AW_INFO("Application minimized."); }
        else { AW_INFO("Application in focus."); }
    }

    void App::OnWindowMove(const WindowMoveEvent& e)
    {
        AW_INFO("Window moved to coordinates {0}, {1}", e.GetNewXPos(), e.GetNewYPos());
    }
}