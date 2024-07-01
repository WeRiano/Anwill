#include "SystemEventHandler.h"
#include "WindowEvents.h"
#include "MouseEvents.h"
#include "KeyEvents.h"

namespace Anwill {

    EventQueue SystemEventHandler::s_SysEvents = EventQueue();

    void SystemEventHandler::Init()
    {
        s_SysEvents.Register<WindowCloseEvent>();
        s_SysEvents.Register<WindowResizeEvent>();
        s_SysEvents.Register<WindowMoveEvent>();
        s_SysEvents.Register<WindowFocusEvent>();

        s_SysEvents.Register<MouseMoveEvent>();
        s_SysEvents.Register<MouseButtonPressEvent>();
        s_SysEvents.Register<MouseButtonReleaseEvent>();
        s_SysEvents.Register<MouseScrollEvent>();
        s_SysEvents.Register<SetMouseCursorEvent>();

        s_SysEvents.Register<KeyPressEvent>();
        s_SysEvents.Register<KeyReleaseEvent>();
        s_SysEvents.Register<KeyRepeatEvent>();
        s_SysEvents.Register<KeyCharEvent>();
    }

    void SystemEventHandler::Pop()
    {
        s_SysEvents.Pop();
    }
}