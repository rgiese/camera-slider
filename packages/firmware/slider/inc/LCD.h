#pragma once

#include "Adafruit_HX8357.h"

class LCD
{
public:
    LCD();

    void begin();

    void updateNumericStatusValue(uint16_t const x, uint16_t const y, RGBColor const color, int32_t const value);

private:
    // Non-copyable
    LCD(LCD const&) = delete;
    LCD& operator=(LCD const&) = delete;

private:
    static constexpr pin_t LCDBrightnessPin = D8;

    Adafruit_HX8357 m_LCD;

private:
    void blitColorRegion(
        uint16_t const x, uint16_t const y, uint16_t const width, uint16_t const height, RGBColor const color);
};