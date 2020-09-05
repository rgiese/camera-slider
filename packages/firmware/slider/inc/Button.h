#pragma once

#include <Arduino.h>
#include <atomic>

class Button
{
public:
    typedef void (*OnButtonPressedFunction)();

public:
    Button(pin_t const pinId, OnButtonPressedFunction const onButtonPressed)
        : m_PinId(pinId)
        , m_OnButtonPressed(onButtonPressed)
        , m_LastInterruptTime(0)
    {
        pinMode(m_PinId, INPUT_PULLUP);
        attachInterrupt(m_PinId, &Button::onInterrupt, this, FALLING);
    }

    ~Button()
    {
        detachInterrupt(m_PinId);
    }

    void onLoop()
    {
        // Deliver onButtonPressed notifications, if any
        while (m_ButtonPressedCount)
        {
            --m_ButtonPressedCount;

            m_OnButtonPressed();
        }
    }

private:
    void onInterrupt()
    {
        // Debounce
        unsigned long const currentTime = millis();
        unsigned long const debounceTime_msec = 5;

        if ((currentTime - m_LastInterruptTime) < debounceTime_msec)
        {
            return;
        }

        m_LastInterruptTime = currentTime;

        // Register onButtonPressed notice to be delivered
        ++m_ButtonPressedCount;
    }

private:
    pin_t const m_PinId;
    OnButtonPressedFunction const m_OnButtonPressed;

    unsigned long m_LastInterruptTime;
    std::atomic<int> m_ButtonPressedCount;

private:
    // Non-copyable
    Button(Button const&) = delete;
    Button& operator=(Button const&) = delete;
};