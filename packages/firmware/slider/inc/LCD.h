#pragma once

#include "Adafruit_HX8357.h"

class LCD
{
public:
    LCD();

    void begin();

private:
    // Non-copyable
    LCD(LCD const&) = delete;
    LCD& operator=(LCD const&) = delete;

private:
    static constexpr pin_t LCDBrightnessPin = D8;

    Adafruit_HX8357 m_LCD;
};