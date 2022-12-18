#include "App.h"

#include "core/Log.h"
#include "core/Window.h"
#include "core/Input.h"

#include "ecs/Ecs.h"

#include "events/MouseEvents.h"

#include "gfx/Renderer.h"
#include "gfx/VertexBuffer.h"

#include "gui/Gui.h"

#include "utils/Random.h"
#include "utils/Profiler.h"

namespace Anwill {

    App::App(const WindowSettings& ws, bool enableGUI)
        : m_Running(true), m_Minimized(false)
    {
        SystemEvents::Init();
        m_Window = Window::Create(ws);
        Input::Init(m_Window->GetNativeWindow());
        Renderer::Init();
        Gui::Init(ws);
        Ecs::Init();
        Random::Init();

        SystemEvents::Subscribe<WindowCloseEvent>(
                AW_BIND_THIS_MEMBER_FUNC(App::OnWindowClose));
        SystemEvents::Subscribe<WindowResizeEvent>(
                AW_BIND_THIS_MEMBER_FUNC(App::OnWindowResize));
        SystemEvents::Subscribe<WindowFocusEvent>(
                AW_BIND_THIS_MEMBER_FUNC(App::OnWindowFocus));
        SystemEvents::Subscribe<WindowMoveEvent>(
                AW_BIND_THIS_MEMBER_FUNC(App::OnWindowMove));

        AW_PROFILE_APP_START();
    }

    App::~App()
    {
        m_Window->Terminate();
        AW_PROFILE_APP_STOP();
        AW_PROFILE_SAVE_TO_DISK();
        //AW_PROFILE_PRINT_TO_CONSOLE();
    }

    void App::Run()
    {
        AW_PROFILE_FUNC();

        while(m_Running)
        {
            Renderer::ClearBuffers();

            m_Window->PreRenderUpdate();

            m_LayerStack.Update();

            Gui::Render();

            m_Window->PostRenderUpdate();

            SystemEvents::Pop();
        }
    }

    void App::OnWindowClose(std::unique_ptr<Event>& event)
    {
        m_Running = false;
    }

    void App::OnWindowResize(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<WindowResizeEvent&>(*event);
        /*AW_INFO("Resized Window to width {0} and height {1}.",
                e.GetNewWidth(), e.GetNewHeight()); */
        Renderer::SetViewport(0, 0, e.GetNewWidth(), e.GetNewHeight());
    }

    void App::OnWindowFocus(std::unique_ptr<Event>& event)
    {
        auto e = static_cast<WindowFocusEvent&>(*event);
        m_Minimized = !e.IsInFocus();
        if(m_Minimized) { AW_INFO("Application minimized."); }
        else { AW_INFO("Application in focus."); }
    }

    void App::OnWindowMove(std::unique_ptr<Event>& event)
    {
        WindowMoveEvent e = static_cast<WindowMoveEvent&>(*event);
        //AW_INFO("Window moved to coordinates {0}, {1}", e.GetNewXPos(), e.GetNewYPos());
    }
}