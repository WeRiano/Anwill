#include "Event.h"

namespace Anwill
{
    EventHandler::EventHandler(Event& event)
        : m_Event(event)
    {}

    EventQueue::EventQueue()
        : m_Head(0), m_Tail(0)
    {}

    void EventQueue::Pop()
    {
        if (m_Head != m_Tail)
        {
            EventType type = m_Q[m_Head]->GetEventType();
            for (unsigned int j = 0; j < m_Subs[type].size(); j++) {
                m_Subs[type][j](m_Q[m_Head]);
            }
            //m_Q[m_Head] = nullptr; // Dont have to do this ... data is left but hidden. TODO: But memory??
            m_Head++;
            m_Head = m_Head % s_Max;
        }
    }

    void EventQueue::Subscribe(std::function<void(std::unique_ptr<Event>&)> func, EventType type)
    {
        m_Subs[type].push_back(func);
    }
}

