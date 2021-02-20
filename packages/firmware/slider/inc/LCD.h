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
        static constexpr int16_t c_idxHighlightCharacter_None = -1;

    public:
        StaticText(LCD& parent,
                   Rect rect,
                   Alignment alignment,
                   GFXfont const* font,
                   RGBColor foregroundColor,
                   RGBColor backgroundColor,
                   uint8_t characterHighlightHeight = 0,
                   uint16_t textYOffset = 0)
            : m_Parent(parent)
            , m_Rect(rect)
            , m_Alignment(alignment)
            , m_Font(font)
            , m_CharacterHighlightHeight(characterHighlightHeight)
            , m_TextYOffset(textYOffset)
            , m_Text()
            , m_idxCharacterToHighlight(c_idxHighlightCharacter_None)
            , m_ForegroundColor(foregroundColor)
            , m_BackgroundColor(backgroundColor){};

        void clear();
        void setText(char const* const szText, int16_t const idxCharacterToHighlight = c_idxHighlightCharacter_None);

        void setForegroundColor(RGBColor const foregroundColor);
        void setBackgroundColor(RGBColor const backgroundColor);

    protected:
        LCD& m_Parent;

        // Immutable properties
        Rect const m_Rect;
        Alignment const m_Alignment;
        GFXfont const* const m_Font;
        uint8_t const m_CharacterHighlightHeight;
        uint16_t const m_TextYOffset;

        // Mutable properties
        char m_Text[32];
        int16_t m_idxCharacterToHighlight;
        RGBColor m_ForegroundColor;
        RGBColor m_BackgroundColor;

    protected:
        void update();
    };

    class StaticNumericText : public StaticText
    {
    public:
        StaticNumericText(LCD& parent,
                          Rect rect,
                          Alignment alignment,
                          GFXfont const* font,
                          RGBColor foregroundColor,
                          RGBColor backgroundColor,
                          uint8_t characterHighlightHeight = 0,
                          uint16_t textYOffset = 0)
            : StaticText(parent,
                         rect,
                         alignment,
                         font,
                         foregroundColor,
                         backgroundColor,
                         characterHighlightHeight,
                         textYOffset)
            , m_Value(std::numeric_limits<decltype(m_Value)>::max())
            , m_ActiveDigit(0){};

        void setValue(int32_t const value);
        void setActiveDigit(uint8_t const activeDigit);

    private:
        // Mutable properties
        int32_t m_Value;
        uint8_t m_ActiveDigit;

        void updateTextFromValueAndActiveDigit();
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
    void blitColorRegion(Rect const& rect, RGBColor const color);

    void blitMonochromeCanvas(uint16_t const x,
                              uint16_t const y,
                              uint16_t const width,
                              uint16_t const height,
                              RGBColor const foregroundColor,
                              RGBColor const backgroundColor);
};