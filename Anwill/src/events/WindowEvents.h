#pragma once

#include "Event.h"

namespace Anwill {

    class WindowCloseEvent : public Event
    {
    public:
        inline static EventType GetEventTypeStatic() { return EventType::WindowClose; }
        inline EventType GetEventType() const override { return EventType::WindowClose; }
    };

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int newWidth, unsigned int newHeight) : m_NewWidth(newWidth), m_NewHeight(newHeight) {}
        inline static EventType GetEventTypeStatic() { return EventType::WindowResize; }
        inline EventType GetEventType() const override { return EventType::WindowResize; }
        inline unsigned int GetNewWidth() { return m_NewWidth; }
        inline unsigned int GetNewHeight() { return m_NewHeight; }

    private:
        unsigned int m_NewWidth, m_NewHeight;
    };

    class WindowFocusEvent : public Event
    {
    public:
        WindowFocusEvent(bool inFocus) : m_InFocus(inFocus) {}
        inline static EventType GetEventTypeStatic() { return EventType::WindowFocus; }
        inline EventType GetEventType() const override { return EventType::WindowFocus; }
        inline bool IsInFocus() { return m_InFocus; }

    private:
        bool m_InFocus;
    };
}