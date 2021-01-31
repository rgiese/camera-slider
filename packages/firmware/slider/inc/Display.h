#pragma once

#include <Adafruit_GFX.h>

class Display
{
public:
    Display()
    {
    }

    void begin();
    void set(char const* const szText);

private:
    // Non-copyable
    Display(Display const&) = delete;
    Display& operator=(Display const&) = delete;
};

extern Display g_Display;
