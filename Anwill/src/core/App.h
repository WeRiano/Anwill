#pragma once

#include <memory>

#include "Window.h"
#include "events/SystemEvents.h"
#include "events/WindowEvents.h"

namespace Anwill {

    class App
    {
    public:
        App();

        void Run();
    private:
        bool m_Running;
        std::unique_ptr<Window> m_Window;

        // Callbacks
        void OnEvent(std::unique_ptr<Event>& event);
        void OnWindowClose(WindowCloseEvent& wce);
        void OnWindowResize(WindowResizeEvent& wre);
        // TODO: More callbacks
        // TODO: App Update / Render events?
    };

    // Client should define this
    App* CreateApp();
}