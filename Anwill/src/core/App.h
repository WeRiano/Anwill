#pragma once

#include <memory>

#include "core/Window.h"
#include "core/LayerStack.h"
#include "events/SystemEvents.h"
#include "events/WindowEvents.h"

namespace Anwill {

    class App
    {
    public:
        App(const WindowSettings& ws);

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

        bool IsMinimized() const;
    private:
        bool m_Running, m_Minimized;
        std::unique_ptr<Window> m_Window;
        LayerStack m_LayerStack;

        // Callbacks
        void OnEvent(std::unique_ptr<Event>& e);
        void OnWindowClose(const WindowCloseEvent& e);
        void OnWindowResize(const WindowResizeEvent& e);
        void OnWindowFocus(const WindowFocusEvent& e);
        void OnWindowMove(const WindowMoveEvent& e);
        // TODO: More callbacks
        // TODO: App Update / Render events?
    };

    // Client should define this
    App* CreateApp();
}