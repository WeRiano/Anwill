#pragma once

#include <memory>
#include <deque>

#include "core/Core.h"
#include "core/Window.h"
#include "core/LayerStack.h"
#include "core/AppStats.h"

#include "events/SystemEventHandler.h"
#include "events/WindowEvents.h"

#include "gfx/Renderer.h"

namespace Anwill {

    struct AppSettings
    {
    public:
        inline AppSettings& SetWindowResolution(unsigned int width, unsigned int height)
        {
            windowSettings.width = width;
            windowSettings.height = height;
            return *this;
        }

        inline AppSettings& SetTitle(const std::string& title)
        {
            windowSettings.title = title.c_str();
            return *this;
        }

        inline AppSettings& SetGraphicsAPI(GraphicsAPI::API api)
        {
            Renderer::SetAPI(api);
            return *this;
        }

        inline AppSettings& SetNumSamples(unsigned int samples)
        {
            windowSettings.samples = samples;
            return *this;
        }

        inline WindowSettings GetWindowSettings() const {
            return windowSettings;
        }

    private:
        WindowSettings windowSettings;
    };

    class App
    {
    public:
        App(const AppSettings& settings);
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

        template <class C>
        void RemoveLayer()
        {
            m_LayerStack.Remove<C>();
        }

    private:
        bool m_Running, m_Minimized;
        Unique<Window> m_Window;
        LayerStack m_LayerStack;

        // Event callbacks
        void OnWindowClose(Unique<Event>& event);
        void OnWindowResize(Unique<Event>& event);
        void OnWindowFocus(Unique<Event>& event);
        void OnWindowMove(Unique<Event>& event);
        // TODO: App Update / Render events?
    };

    // Client should define this
    App* CreateApp();
}