#include "Event.h"

namespace Anwill {

    class SystemEvents
    {
    public:
        static void Init();

        template<typename E>
        static void Add(E const& child)
        {
            s_SysEvents.Add<E>(child);
        }
        static void Pop();

        template <class E>
        static void Subscribe(std::function<void(std::unique_ptr<Event>&)> func)
        {
            s_SysEvents.Subscribe<E>(func);
        }

    private:
        static EventQueue s_SysEvents;
    };
}