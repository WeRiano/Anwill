#include "App.h"

#include <glad.h>

#include "core/Log.h"
#include "core/Window.h"
#include "gfx/Renderer.h"

namespace Anwill {

    App::App()
        : m_Running(true), m_Minimized(false)
    {
        // TODO: Renderer has to be initialized before GraphicsContext because of API def. Fix?
        Renderer::SetAPI(GraphicsAPI::API::OpenGL); // TODO: PARAMETER IS RETARDED FOR USER TO SET!!!!

        WindowSettings ws {1600, 900}; // TODO: Import from Sandbox (?)
        m_Window = Window::Create(ws);

        Renderer::Init();

        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowClose);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowResize);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowFocus);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowMoved);

    }

    void App::Run()
    {
        while(m_Running)
        {
            m_LayerStack.Update();
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(-0.3f, 0.7f, 1.0f, 1.0f);
            unsigned int k = 10;
            glCreateBuffers(-10, &k);
            m_Window->Update();
            SystemEvents::Pop();
        }
        m_Window->Terminate();
    }

    void App::OnEvent(std::unique_ptr<Event>& e)
    {
        EventHandler handler(*e);
        handler.Handle<WindowCloseEvent>(AW_BIND_EVENT_FN(OnWindowClose));
        handler.Handle<WindowResizeEvent>(AW_BIND_EVENT_FN(OnWindowResize));
        handler.Handle<WindowFocusEvent>(AW_BIND_EVENT_FN(OnWindowFocus));
    }

    void App::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
    }

    void App::OnWindowResize(WindowResizeEvent& e)
    {
        AW_INFO("Resized Window to width {0} and height {1}.", e.GetNewWidth(), e.GetNewHeight());
        glViewport(0, 0, e.GetNewWidth(), e.GetNewHeight()); // TODO: Move to renderer
    }

    void App::OnWindowFocus(WindowFocusEvent e)
    {
        m_Minimized = !e.IsInFocus();
        if(m_Minimized) { AW_INFO("Application mininized."); }
        else { AW_INFO("Application in focus."); }
    }
}