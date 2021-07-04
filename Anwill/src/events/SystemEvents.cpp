#include "SystemEvents.h"

#include <utility>

namespace Anwill {
    EventQueue SystemEvents::s_SysEvents = EventQueue();

    void SystemEvents::Pop()
    {
        s_SysEvents.Pop();
    }

    void SystemEvents::Subscribe(std::function<void(std::unique_ptr<Event>&)> func, EventType type)
    {
        s_SysEvents.Subscribe(std::move(func), type);
    }
}