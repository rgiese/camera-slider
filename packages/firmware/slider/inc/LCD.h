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

    enum class Alignment : uint8_t
    {
        Left,
        Center,
        Right
    };

    class StaticText
    {
    public:
        StaticText(LCD& parent,
                   Rect rect,
                   Alignment alignment,
                   GFXfont const* font,
                   RGBColor foregroundColor,
                   RGBColor backgroundColor)
            : m_Parent(parent)
            , m_Rect(rect)
            , m_Alignment(alignment)
            , m_Font(font)
            , m_ForegroundColor(foregroundColor)
            , m_BackgroundColor(backgroundColor){};

        void setText(std::string const& text) const;

    private:
        LCD& m_Parent;
        Rect const m_Rect;
        Alignment const m_Alignment;
        GFXfont const* const m_Font;
        RGBColor const m_ForegroundColor;
        RGBColor const m_BackgroundColor;
    };

public:
    LCD();

    void begin();

    static constexpr uint16_t DisplayWidth = HX8357_TFTHEIGHT;  // (rotated)
    static constexpr uint16_t DisplayHeight = HX8357_TFTWIDTH;

private:
    // Non-copyable
    LCD(LCD const&) = delete;
    LCD& operator=(LCD const&) = delete;

private:
    static constexpr pin_t LCDBrightnessPin = D8;

    Adafruit_HX8357 m_LCD;
    GFXcanvas1 m_MonochromeCanvas;

private:
    void blitColorRegion(
        uint16_t const x, uint16_t const y, uint16_t const width, uint16_t const height, RGBColor const color);

    void blitMonochromeCanvas(uint16_t const x,
                              uint16_t const y,
                              uint16_t const width,
                              uint16_t const height,
                              RGBColor const foregroundColor,
                              RGBColor const backgroundColor);
};