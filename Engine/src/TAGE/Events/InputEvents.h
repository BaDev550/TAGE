#pragma once

#include "TAGE/Events/Input.h"
#include "TAGE/Events/Event.h"

namespace TAGE {
    class KeyPressedEvent : public Event {
    public:
        KeyPressedEvent(KeyCode key)
            : m_Key(key) {
        }

        KeyCode GetKey() const { return m_Key; }

        EVENT_CLASS_TYPE(KeyPressed);
    private:
        KeyCode m_Key;
    };

    class KeyReleasedEvent : public Event {
    public:
        KeyReleasedEvent(KeyCode key)
            : m_Key(key) {
        }

        KeyCode GetKey() const { return m_Key; }
        EVENT_CLASS_TYPE(KeyReleased);
    private:
        KeyCode m_Key;
    };

    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y)
            : m_X(x), m_Y(y) {
        }

        float GetX() const { return m_X; }
        float GetY() const { return m_Y; }

        EVENT_CLASS_TYPE(MouseMoved);
    private:
        float m_X, m_Y;
    };

    class MouseButtonPressedEvent : public Event {
    public:
        MouseButtonPressedEvent(MouseButton button)
            : m_Button(button) {
        }

        MouseButton GetButton() const { return m_Button; }

        EVENT_CLASS_TYPE(MouseButtonPressed);
    private:
        MouseButton m_Button;
    };

    class MouseButtonReleasedEvent : public Event {
    public:
        MouseButtonReleasedEvent(MouseButton button)
            : m_Button(button) {
        }

        MouseButton GetButton() const { return m_Button; }
         
        EVENT_CLASS_TYPE(MouseButtonReleased);
    private:
        MouseButton m_Button;
    };

    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {
        }

        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }

        EVENT_CLASS_TYPE(MouseScrolled);
    private:
        float m_XOffset, m_YOffset;
    };
}