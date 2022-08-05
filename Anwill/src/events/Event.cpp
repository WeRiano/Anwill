#include "Event.h"

namespace Anwill
{
    EventTypeID EventQueue::s_NewEventTypeIDCount = 0;

    EventQueue::EventQueue()
        : m_Head(0), m_Tail(0), m_PopSize(0)
    {}

    void EventQueue::Pop()
    {
        CalcPopSize();

        for(unsigned int i = 0; i < m_PopSize; i++)
        {
            if (m_Head != m_Tail)
            {
                EventTypeID id = m_Q[m_Head].first;
                // If there are no subs, callbacks will be an empty vector (size 0)
                auto& callbacks = m_Subs[id];
                for (const auto& callback: callbacks)
                {
                    callback(m_Q[m_Head].second);
                }
                //m_Q[m_Head] = nullptr; // Dont have to do this ... data is left but hidden.
                m_Head++;
                m_Head = m_Head % s_Max;
            }
        }
    }

    EventTypeID EventQueue::CreateNewEventTypeID()
    {
        s_NewEventTypeIDCount++;
        return s_NewEventTypeIDCount;
    }

    void EventQueue::CalcPopSize()
    {
        int diff = m_Tail - m_Head;
        unsigned int queueSize = std::abs(diff);
        // Mmm very dynamic so complicated.
        m_PopSize = queueSize;
    }
}

