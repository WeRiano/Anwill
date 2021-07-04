#include "Event.h"

namespace Anwill {
    class SystemEvents
    {
    public:
        template<typename C>
        static void Add(C const& child)
        {
            s_SysEvents.Add<C>(child);
        }
        static void Pop();
        static void Subscribe(std::function<void(std::unique_ptr<Event>&)> func, EventType type);

    private:
        static EventQueue s_SysEvents;
    };
}