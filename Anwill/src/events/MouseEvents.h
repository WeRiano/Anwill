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
        inline const MouseButtonCode GetMouseCode() const { return m_MouseButtonCode; }
    private:
        MouseButtonCode m_MouseButtonCode;
    };

    class MouseButtonReleaseEvent : public Event {
    public:
        MouseButtonReleaseEvent(MouseButtonCode mouseButtonCode)
            : m_MouseButtonCode(mouseButtonCode) {}
        inline const MouseButtonCode GetMouseCode() const { return m_MouseButtonCode; }
    private:
        MouseButtonCode m_MouseButtonCode;
    };

    class MouseScrollEvent : public Event {
    public:
        MouseScrollEvent(MouseScrollCode mouseScrollCode)
            : m_MouseScrollCode(mouseScrollCode) {}
        inline const MouseScrollCode GetScrollCode() const { return m_MouseScrollCode; }
    private:
        MouseScrollCode m_MouseScrollCode;
    };

    class SetMouseCursorEvent : public Event {
    public:
        enum class CursorType : int {
            Arrow = 0,
            HorizontalResize,
            VerticalResize,
            PositiveDiagonalResize,
            NegativeDiagonalResize,
            TextInput,
            PointingHand,
            GrabbingHand,

            NumberOfCursorTypes
        };

        SetMouseCursorEvent(CursorType cursorType)
            : m_CursorType(cursorType) {}
        inline const CursorType GetCursorType() const { return m_CursorType; }
    private:
        CursorType m_CursorType;
    };
}