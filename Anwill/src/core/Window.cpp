#include <memory>

#include "Window.h"

#ifdef AW_PLATFORM_WINDOWS
#include "platform/WinWindow.h"
#endif

namespace Anwill {

    std::unique_ptr<Window> Window::Create(WindowSettings ws)
    {
        #ifdef AW_PLATFORM_WINDOWS
            return std::make_unique<WinWindow>();
        #else
            #error "Window platform not supported."
        #endif
    }

}
