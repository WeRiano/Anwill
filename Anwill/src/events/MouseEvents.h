#pragma once

#include "core/MouseCodes.h"
#include "events/Event.h"

namespace Anwill {

    class MouseMoveEvent : public Event {
    public:
        MouseMoveEvent(float mouseXPos, float mouseYPos) : m_XPos(mouseXPos), m_YPos(mouseYPos) {}
        inline static EventType GetEventTypeStatic() { return EventType::MouseMove; }
        inline EventType GetEventType() const override { return EventType::MouseMove; }
        inline const float GetXPos() const { return m_XPos; }
        inline const float GetYPos() const { return m_YPos; }
    private:
        float m_XPos, m_YPos;
    };

    class MouseButtonPressEvent : public Event {
    public:
        MouseButtonPressEvent(MouseButtonCode mouseButtonCode) : m_MouseButtonCode(mouseButtonCode) {}
        inline static EventType GetEventTypeStatic() { return EventType::MouseButtonPress; }
        inline EventType GetEventType() const override { return EventType::MouseButtonPress; }
        inline const MouseButtonCode GetKeyCode() const { return m_MouseButtonCode; }
    private:
        MouseButtonCode m_MouseButtonCode;
    };

    class MouseButtonReleaseEvent : public Event {
    public:
        MouseButtonReleaseEvent(MouseButtonCode mouseButtonCode) : m_MouseButtonCode(mouseButtonCode) {}
        inline static EventType GetEventTypeStatic() { return EventType::MouseButtonRelease; }
        inline EventType GetEventType() const override { return EventType::MouseButtonRelease; }
        inline const MouseButtonCode GetKeyCode() const { return m_MouseButtonCode; }
    private:
        MouseButtonCode m_MouseButtonCode;
    };
}