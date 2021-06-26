#pragma once

#include <memory>

#include "Window.h"

namespace Anwill {

    class App
    {
    public:
        App();

    private:
        bool m_Running;
        std::unique_ptr<Window> m_Window;

        void Run();
    };

    // Client should define this
    App* CreateApp();
}