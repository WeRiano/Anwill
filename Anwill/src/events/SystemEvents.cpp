#include "SystemEvents.h"
#include "WindowEvents.h"
#include "MouseEvents.h"
#include "KeyEvents.h"

namespace Anwill {
    EventQueue SystemEvents::s_SysEvents = EventQueue();

    void SystemEvents::Init()
    {
        s_SysEvents.Register<WindowCloseEvent>();
        s_SysEvents.Register<WindowResizeEvent>();
        s_SysEvents.Register<WindowMoveEvent>();
        s_SysEvents.Register<WindowFocusEvent>();

        s_SysEvents.Register<MouseMoveEvent>();
        s_SysEvents.Register<MouseButtonPressEvent>();
        s_SysEvents.Register<MouseButtonReleaseEvent>();

        s_SysEvents.Register<KeyPressEvent>();
        s_SysEvents.Register<KeyReleaseEvent>();
        s_SysEvents.Register<KeyRepeatEvent>();
    }

    void SystemEvents::Pop()
    {
        s_SysEvents.Pop();
    }
}