#include "App.h"

#include "core/Log.h"
#include "core/Window.h"
#include "core/Input.h"

#include "ecs/Ecs.h"

#include "events/MouseEvents.h"

#include "gfx/Renderer.h"
#include "gfx/VertexBuffer.h"

#include "imgui/Imgui.h"

#include "utils/Random.h"
#include "utils/Profiler.h"

namespace Anwill {

    Timestamp AppStats::layerStackAverageUpdateDuration = Timestamp(0);
    std::deque<Timestamp> AppStats::layerStackUpdateDurations;

    App::App(const AppSettings& settings)
        : m_Running(true), m_Minimized(false)
    {
        SystemEventHandler::Init();

        WindowSettings windowSettings = settings.GetWindowSettings();
        m_Window = Window::Create(windowSettings);

        Input::Init(m_Window->GetNativeWindow());

        Renderer::Init();

        Imgui::Init(*m_Window.get());

        Ecs::Init();

        Random::Init();

        SystemEventHandler::Subscribe<WindowCloseEvent>(
                AW_BIND_THIS_MEMBER_FUNC(App::OnWindowClose));
        SystemEventHandler::Subscribe<WindowResizeEvent>(
                AW_BIND_THIS_MEMBER_FUNC(App::OnWindowResize));
        SystemEventHandler::Subscribe<WindowFocusEvent>(
                AW_BIND_THIS_MEMBER_FUNC(App::OnWindowFocus));
        SystemEventHandler::Subscribe<WindowMoveEvent>(
                AW_BIND_THIS_MEMBER_FUNC(App::OnWindowMove));

        AW_PROFILE_APP_START();
    }

    App::~App()
    {
        m_Window->Terminate();
        AW_PROFILE_APP_STOP();
        AW_PROFILE_SAVE_TO_DISK();
        AW_PROFILE_PRINT_TO_CONSOLE();
    }

    void App::Run()
    {
        AW_PROFILE_FUNC();

        while(m_Running)
        {
            Renderer::ClearBuffers();

            m_Window->PreRenderUpdate();

            Timestamp start;
            m_LayerStack.Update();
            Timestamp end;
            Imgui::NewFrame();
            m_LayerStack.ImguiUpdate();
            AppStats::PushUpdateDuration(end-start);

            Imgui::EndFrame();

            Imgui::Render();

            m_Window->PostRenderUpdate();

            SystemEventHandler::Pop();
        }
    }

    void App::OnWindowClose(std::unique_ptr<Event>& event)
    {
        m_Running = false;
    }

    void App::OnWindowResize(Unique<Event>& event)
    {
        auto e = static_cast<WindowResizeEvent&>(*event);
        AW_INFO("Resized Window to width {0} and height {1}.", e.GetNewWidth(), e.GetNewHeight());
        Renderer::SetViewport(0, 0, e.GetNewWidth(), e.GetNewHeight());
        Renderer::SetScissor({}, {(float)e.GetNewWidth(), (float)e.GetNewHeight()});
    }

    void App::OnWindowFocus(Unique<Event>& event)
    {
        auto e = static_cast<WindowFocusEvent&>(*event);
        m_Minimized = !e.IsInFocus();
        if(m_Minimized) { AW_INFO("Application minimized."); }
        else { AW_INFO("Application in focus."); }
    }

    void App::OnWindowMove(Unique<Event>& event)
    {
        WindowMoveEvent e = static_cast<WindowMoveEvent&>(*event);
        AW_INFO("Window moved to coordinates {0}, {1}", e.GetNewXPos(), e.GetNewYPos());
    }
}