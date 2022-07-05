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
        inline unsigned int GetNewWidth() const { return m_NewWidth; }
        inline unsigned int GetNewHeight() const { return m_NewHeight; }

    private:
        unsigned int m_NewWidth, m_NewHeight;
    };

    class WindowMoveEvent : public Event
    {
    public:
        WindowMoveEvent(int newXPos, int newYPos) : m_NewXPos(newXPos), m_NewYPos(newYPos) {}
        inline static EventType GetEventTypeStatic() { return EventType::WindowMove; }
        inline EventType GetEventType() const override { return EventType::WindowMove; }
        inline int GetNewXPos() const { return m_NewXPos; }
        inline int GetNewYPos() const { return m_NewYPos; }

    private:
        int m_NewXPos, m_NewYPos;
    };

    class WindowFocusEvent : public Event
    {
    public:
        WindowFocusEvent(bool inFocus) : m_InFocus(inFocus) {}
        inline static EventType GetEventTypeStatic() { return EventType::WindowFocus; }
        inline EventType GetEventType() const override { return EventType::WindowFocus; }
        inline bool IsInFocus() const { return m_InFocus; }

    private:
        bool m_InFocus;
    };
}