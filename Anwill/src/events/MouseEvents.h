#pragma once

#include "core/MouseCodes.h"
#include "events/Event.h"

namespace Anwill {

    class MouseMoveEvent : public Event {
    public:
        MouseMoveEvent(float mouseXPos, float mouseYPos)
        : m_XPos(mouseXPos), m_YPos(mouseYPos) {}
        inline const float GetXPos() const { return m_XPos; }
        inline const float GetYPos() const { return m_YPos; }
    private:
        float m_XPos, m_YPos;
    };

    class MouseButtonPressEvent : public Event {
    public:
        MouseButtonPressEvent(MouseButtonCode mouseButtonCode)
        : m_MouseButtonCode(mouseButtonCode) {}
        inline const MouseButtonCode GetKeyCode() const { return m_MouseButtonCode; }
    private:
        MouseButtonCode m_MouseButtonCode;
    };

    class MouseButtonReleaseEvent : public Event {
    public:
        MouseButtonReleaseEvent(MouseButtonCode mouseButtonCode)
        : m_MouseButtonCode(mouseButtonCode) {}
        inline const MouseButtonCode GetKeyCode() const { return m_MouseButtonCode; }
    private:
        MouseButtonCode m_MouseButtonCode;
    };
}