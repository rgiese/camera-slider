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
                   RGBColor backgroundColor,
                   uint8_t characterHighlightHeight = 0)
            : m_Parent(parent)
            , m_Rect(rect)
            , m_Alignment(alignment)
            , m_Font(font)
            , m_ForegroundColor(foregroundColor)
            , m_BackgroundColor(backgroundColor)
            , m_CharacterHighlightHeight(characterHighlightHeight){};

        void setText(char const* const szText, ssize_t const idxCharacterToHighlight = -1) const;

    protected:
        LCD& m_Parent;
        Rect const m_Rect;
        Alignment const m_Alignment;
        GFXfont const* const m_Font;
        RGBColor const m_ForegroundColor;
        RGBColor const m_BackgroundColor;
        uint8_t m_CharacterHighlightHeight;
    };

    class StaticNumericText : private StaticText
    {
    public:
        StaticNumericText(LCD& parent,
                          Rect rect,
                          Alignment alignment,
                          GFXfont const* font,
                          RGBColor foregroundColor,
                          RGBColor backgroundColor,
                          uint8_t characterHighlightHeight = 0)
            : StaticText(parent, rect, alignment, font, foregroundColor, backgroundColor, characterHighlightHeight)
            , m_Value(std::numeric_limits<decltype(m_Value)>::max())
            , m_ActiveDigit(0){};

        void setValue(int32_t const value);
        void setActiveDigit(uint8_t const activeDigit);

    private:
        int32_t m_Value;
        uint8_t m_ActiveDigit;

        void update();
    };

public:
    LCD();

    void begin();

    static constexpr uint16_t DisplayWidth = HX8357_TFTHEIGHT;  // (rotated)
    static constexpr uint16_t DisplayHeight = HX8357_TFTWIDTH;

public:
    void blitColorRegion(Rect const& rect, RGBColor const color);

    void drawText(char const* const szText,
                  Rect const& rect,
                  Alignment const alignment,
                  GFXfont const* const pFont,
                  RGBColor const foregroundColor,
                  RGBColor const backgroundColor,
                  uint8_t const characterHighlightHeight,
                  ssize_t const idxCharacterToHighlight);

private:
    // Non-copyable
    LCD(LCD const&) = delete;
    LCD& operator=(LCD const&) = delete;

private:
    static constexpr pin_t LCDBrightnessPin = D8;

    Adafruit_HX8357 m_LCD;
    GFXcanvas1 m_MonochromeCanvas;

private:
    void blitMonochromeCanvas(uint16_t const x,
                              uint16_t const y,
                              uint16_t const width,
                              uint16_t const height,
                              RGBColor const foregroundColor,
                              RGBColor const backgroundColor);
};