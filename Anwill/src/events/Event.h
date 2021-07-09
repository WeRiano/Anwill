#pragma once

#include <functional>
#include <array>

#define AW_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Anwill {

    enum EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, /*WindowLostFocus,*/ WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
        EventTypeMax // Used to count the number of different events!
    };

    class Event
    {
    public:
        virtual EventType GetEventType() const = 0;

    private:
        bool m_Handled;
    };

    class EventHandler
    {
    public:
        EventHandler(Event& event);

        template<typename C, typename F>
        void Handle(F func)
        {
            if(m_Event.GetEventType() == C::GetEventTypeStatic()) {
                func(static_cast<C&>(m_Event));
            }
        }

    private:
        Event& m_Event;
    };

    class EventQueue
    {
    public:
        EventQueue();

        template<typename C>
        void Add(C const& child)
        {
            auto copy = std::make_unique<C>(child);
            m_Q[m_Tail] = std::move(copy);
            m_Tail++;
            m_Tail = m_Tail % s_Max;
        }
        // Pop the event next in line and call all of its subscribers
        void Pop();
        void Subscribe(std::function<void(std::unique_ptr<Event>&)> func, EventType type);
        //void Unsubscribe(); // Needed ever?

    private:
        unsigned int m_Head, m_Tail;
        static const unsigned int s_Max = 20000;
        std::array<std::unique_ptr<Event>, s_Max> m_Q;
        std::array<std::vector<std::function<void(std::unique_ptr<Event>&)>>, EventType::EventTypeMax> m_Subs;
    };

}