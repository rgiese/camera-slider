#pragma once


class UI
{
public:
    UI();

    void begin();
    void onMainLoop();

private:
    // Non-copyable
    UI(UI const&) = delete;
    UI& operator=(UI const&) = delete;

private:
    //
    // Shared LCD/Encoder
    //

    enum class EncoderFunction : uint8_t
    {
        Position,
        Speed,
        Acceleration,
        Step,
        Rate,
        __count
    };

    RGBColor m_FunctionColors[static_cast<size_t>(EncoderFunction::__count)];

    constexpr RGBColor const& colorFor(EncoderFunction const encoderFunction)
    {
        return m_FunctionColors[static_cast<uint8_t>(encoderFunction)];
    };

private:
    //
    // LCD
    //

    LCD m_LCD;

    struct LCDConstants
    {
        static constexpr uint16_t PositionSpeedAcceleration_Width = 128;
        static constexpr uint16_t PositionSpeedAcceleration_Height = 32;
        static constexpr uint16_t PositionSpeedAcceleration_HighlightHeight = 2;
        static constexpr uint16_t PositionSpeedAcceleration_Y = LCD::DisplayHeight - PositionSpeedAcceleration_Height;
    };

    GFXfont const* const m_PositionSpeedAcceleration_Font;

    LCD::StaticNumericText m_PositionText;
    LCD::StaticNumericText m_SpeedText;
    LCD::StaticNumericText m_AccelerationText;

private:
    //
    // Encoders
    //

    TwoWire& m_Wire;

    static constexpr pin_t EncoderInterruptPin = D4;

    Encoder m_Encoders[static_cast<size_t>(EncoderFunction::__count)];

    constexpr Encoder& encoderFor(EncoderFunction const encoderFunction)
    {
        return m_Encoders[static_cast<uint8_t>(encoderFunction)];
    };
};

extern UI g_UI;