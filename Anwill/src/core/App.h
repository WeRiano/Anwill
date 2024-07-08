#pragma once

#include <memory>

#include "core/Window.h"
#include "core/LayerStack.h"

#include "events/SystemEventHandler.h"
#include "events/WindowEvents.h"

namespace Anwill {

    class App
    {
    public:
        App(const WindowSettings& ws);
        ~App();

        void Run();

        /**
         * Add a layer to the application
         * @tparam C - Layer child class
         * @param ups - updates per second.
         * @param args - arguments to give to the layer
         */
        template <class C, typename... Args>
        void AddLayer(const unsigned int ups, Args&&... args)
        {
            m_LayerStack.Add<C>(ups, std::forward<Args>(args)...);
        }

    private:
        bool m_Running, m_Minimized;
        std::unique_ptr<Window> m_Window;
        LayerStack m_LayerStack;

        // Event callbacks
        void OnWindowClose(std::unique_ptr<Event>& event);
        void OnWindowResize(std::unique_ptr<Event>& event);
        void OnWindowFocus(std::unique_ptr<Event>& event);
        void OnWindowMove(std::unique_ptr<Event>& event);
        // TODO: App Update / Render events?
    };

    // Client should define this
    App* CreateApp();
}