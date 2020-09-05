#pragma once

class Display
{
public:
    Display()
        : m_Display(128, 32, &Wire)
    {
    }

    void begin();
    void set(char const* const szText);

private:
    Adafruit_SSD1306 m_Display;

private:
    // Non-copyable
    Display(Display const&) = delete;
    Display& operator=(Display const&) = delete;
};

extern Display g_Display;
