#pragma once

#include "Event.h"
#include "core/KeyCodes.h"

namespace Anwill {

    class KeyPressEvent : public Event {
    public:
        KeyPressEvent(KeyCode keyCode) : m_KeyCode(keyCode) {}
        inline const KeyCode GetKeyCode() const { return m_KeyCode; }
    private:
        KeyCode m_KeyCode;
    };

    class KeyReleaseEvent : public Event {
    public:
        KeyReleaseEvent(KeyCode keyCode) : m_KeyCode(keyCode) {}
        inline const KeyCode GetKeyCode() const { return m_KeyCode; }
    private:
        KeyCode m_KeyCode;
    };

    class KeyRepeatEvent : public Event {
    public:
        KeyRepeatEvent(KeyCode keyCode) : m_KeyCode(keyCode) {}
        inline const KeyCode GetKeyCode() const { return m_KeyCode; }
    private:
        KeyCode m_KeyCode;
    };

}