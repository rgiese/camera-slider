#pragma once


class UI
{
public:
    UI();

    void begin();

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
        static constexpr uint16_t PositionSpeedAcceleration_Height = 40;
        static constexpr uint16_t PositionSpeedAcceleration_Y = LCD::DisplayHeight - PositionSpeedAcceleration_Height;
    };

    LCD::StaticText m_PositionText;
    LCD::StaticText m_SpeedText;
    LCD::StaticText m_AccelerationText;

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