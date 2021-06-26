#include "App.h"

#include "Window.h"

namespace Anwill {

    App::App()
    {
        WindowSettings ws {};
        // ws.width
        // ...
        m_Window = Window::Create(ws);
    }

    void App::Run()
    {
        while(m_Running)
        {
            //if(m_Window->OnMsg)
        }
    }
}