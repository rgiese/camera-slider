#pragma once

#include "Adafruit_HX8357.h"

class LCD
{
public:
    struct Rect
    {
        uint16_t X;
        uint16_t Y;
        uint16_t Width;
        uint16_t Height;
    };

    class StaticText
    {
    public:
        StaticText(LCD& parent, Rect rect, RGBColor color)
            : m_Parent(parent)
            , m_Rect(rect)
            , m_Color(color){};

        void setText(std::string const& text) const;

    private:
        LCD& m_Parent;
        Rect m_Rect;
        RGBColor m_Color;
    };

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

private:
    void blitColorRegion(
        uint16_t const x, uint16_t const y, uint16_t const width, uint16_t const height, RGBColor const color);
};