#pragma once

#include "Event.h"

namespace Anwill {

    struct GuiLoseFocusEvent : public Event {};

    class GuiEventHandler
    {
    public:
        static void Init();

        template<typename E>
        static void Add(E const& child)
        {
            s_GuiEvents.Add<E>(child);
        }
        static void Pop();

        template <class E>
        static void Subscribe(std::function<void(std::unique_ptr<Event>&)> func)
        {
            s_GuiEvents.Subscribe<E>(func);
        }

    private:
        static EventQueue s_GuiEvents;
    };
}
