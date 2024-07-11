#include "EditorEventHandler.h"

namespace Anwill {

    EventQueue EditorEventHandler::s_SysEvents = EventQueue();

    void EditorEventHandler::Init()
    {
        s_SysEvents.Register<StartTestEnvironmentEvent>();
    }

    void EditorEventHandler::Pop()
    {
        s_SysEvents.Pop();
    }
}