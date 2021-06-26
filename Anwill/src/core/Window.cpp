#include <memory>

#include "Window.h"

#ifdef AW_PLATFORM_WINDOWS
#include "platform/WinWindow.h"
#endif

namespace Anwill {

    std::unique_ptr<Window> Window::Create(const WindowSettings ws)
    {
        #ifdef AW_PLATFORM_WINDOWS
            return std::make_unique<WinWindow>(ws);
        #else
            #error "Window platform not supported."
        #endif
    }

}
