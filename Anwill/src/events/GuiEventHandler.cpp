#include "GuiEventHandler.h"

namespace Anwill {

    EventQueue GuiEventHandler::s_GuiEvents = EventQueue();

    void GuiEventHandler::Init()
    {
        s_GuiEvents.Register<GuiLoseFocusEvent>();
    }

    void GuiEventHandler::Pop()
    {
        s_GuiEvents.Pop();
    }

}