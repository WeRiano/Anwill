#include "GuiEvents.h"

namespace Anwill {

    EventQueue GuiEvents::s_GuiEvents = EventQueue();

    void GuiEvents::Init()
    {
        s_GuiEvents.Register<GuiLoseFocusEvent>();
    }

    void GuiEvents::Pop()
    {
        s_GuiEvents.Pop();
    }

}