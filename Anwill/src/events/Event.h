#pragma once

#include "core/Log.h"
#include "core/Assert.h"

#include <functional>
#include <array>
#include <unordered_map>
#include <typeindex>
#include <type_traits>

// Need to use this to pass a non-static member function as a parameter.
// Member functions need to be called on an object which is passed as an implicit
// argument to the function. If you want to bind a non-static member
// function, global function or lambda, don't use this. Just pass it like normal.
#define AW_BIND_THIS_MEMBER_FUNC(fn) [this](auto&&... args) -> decltype(auto) \
{ return this->fn(std::forward<decltype(args)>(args)...); }

namespace Anwill {

    typedef unsigned long EventTypeID;

    class Event
    {
    private:
        //bool m_Handled; TODO: Mark event as handled feature
    };

    class EventQueue
    {
    public:
        EventQueue();

        template<typename E>
        void Add(E const& event)
        {
            std::type_index typeIndex = std::type_index(typeid(E));
            AW_ASSERT(m_IDs.contains(typeIndex),
                      "Can't add an event that has not been registered.");
            EventTypeID id = m_IDs.at(typeIndex);
            std::unique_ptr<E> ptr = std::make_unique<E>(event);
            m_Q[m_Tail] = {id, std::move(ptr)};
            m_Tail++;
            m_Tail = m_Tail % s_Max;
        }

        template <typename E>
        void Subscribe(std::function<void(std::unique_ptr<Event>&)> func)
        {
            std::type_index typeIndex = std::type_index(typeid(E));
            AW_ASSERT(m_IDs.contains(typeIndex),
                      "Can't subscribe to an event that has not been registered.");
            EventTypeID id = m_IDs.at(typeIndex);
            m_Subs[id].emplace_back(func);
        }

        template <typename E>
        void Register() {
            AW_ASSERT((std::is_base_of<Event, E>::value),
                      "Tried to register a custom event class that does not extend "
                      "Anwill::Event!");
            auto typeIndex = std::type_index(typeid(E));
            if(!m_IDs.contains(typeIndex)) {
                m_IDs[typeIndex] = CreateNewEventTypeID();
            } else {
                AW_ERROR("Tried to register {0} twice!", typeid(E).name());
            }
        }

        void Pop();
        //void Unsubscribe(); TODO: Unsubscribe feature

    private:
        static const unsigned int s_Max = 20000;
        static EventTypeID s_NewEventTypeIDCount;

        unsigned int m_Head, m_Tail, m_PopSize;
        std::array<std::pair<EventTypeID, std::unique_ptr<Event>>, s_Max> m_Q;
        std::unordered_map<EventTypeID,
        std::vector<std::function<void(std::unique_ptr<Event>&)>>> m_Subs;
        std::unordered_map<std::type_index, EventTypeID> m_IDs;

        /**
         * @brief This will get the id from the template type or create one if it has
         *        yet not encountered this type of event. Nice if we don't want the user
         *        to have to think about registering custom events, but a lot slower.
         */
        template <class E>
        EventTypeID CreateOrGetEventTypeID() {
            AW_ASSERT((std::is_base_of<Event, E>::value),
                      "Tried to get or create an id for a custom event class "
                      "that does not extend Anwill::Event!");

            auto typeIndex = std::type_index(typeid(E));
            EventTypeID id;
            if (m_IDs.contains(typeIndex)) {
                id = m_IDs[typeIndex];
            } else {
                id = CreateNewEventTypeID();
                m_IDs[typeIndex] = id;
            }
            return id;
        }

        static EventTypeID CreateNewEventTypeID();
        void CalcPopSize();
    };

    // This exists so that you do not have to switch or if spam event type in your
    // callback that you pass when you
    // subscribe, if you decide to handle all functions in a single "OnEvent" func.
    // But why not just subscribe directly with your EventHandler funcs??
    // DEPRECATED (for now):
    /*
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
     */
}