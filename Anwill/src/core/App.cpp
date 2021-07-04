#include "App.h"

#include "core/Log.h"
#include "core/Window.h"
#include "gfx/Renderer.h"
#include "platform/OpenGL/OpenGLBase.h"

#include <windows.h> // TODO: TEMP, REMOVE

namespace Anwill {

    App::App()
        : m_Running(true)
    {
        // TODO: Renderer has to be initialized before GraphicsContext because of API def. Fix?
        Renderer::Init(Anwill::OpenGL);
        WindowSettings ws {1600, 900};
        m_Window = Window::Create(ws);

        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowClose);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowResize);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowFocus);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowLostFocus);
        SystemEvents::Subscribe(AW_BIND_EVENT_FN(App::OnEvent), EventType::WindowMoved);

    }

    void App::Run()
    {
        while(m_Running)
        {
            Sleep(10); // TODO: TEMP, REMOVE
            GLCall(glClear(GL_COLOR_BUFFER_BIT));
            GLCall(glClearColor(0.3f, 0.7f, 1.0f, 1.0f));
            m_Window->Update();
            SystemEvents::Pop();
        }
        m_Window->Terminate();
    }

    void App::OnEvent(std::unique_ptr<Event>& event)
    {
        EventHandler handler(*event);
        handler.Handle<WindowCloseEvent>(AW_BIND_EVENT_FN(OnWindowClose));
        handler.Handle<WindowResizeEvent>(AW_BIND_EVENT_FN(OnWindowResize));
    }

    void App::OnWindowClose(WindowCloseEvent& wce)
    {
        m_Running = false;
    }

    void App::OnWindowResize(WindowResizeEvent& wre)
    {
        AW_INFO("Resized window to width {0} and height {1}", wre.GetNewWidth(), wre.GetNewHeight());
        GLCall(glViewport(0, 0, wre.GetNewWidth(), wre.GetNewHeight())); // TODO: Renderer call
    }
}