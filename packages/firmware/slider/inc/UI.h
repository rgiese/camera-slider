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
        static constexpr uint16_t DesiredMovementParameters_Width = 128;
        static constexpr uint16_t DesiredMovementParameters_Height = 32;
        static constexpr uint16_t DesiredMovementParameters_HighlightHeight = 2;
        static constexpr uint16_t DesiredMovementParameters_Y = LCD::DisplayHeight - DesiredMovementParameters_Height;

        static constexpr uint16_t ReportedMovementParameters_Width = DesiredMovementParameters_Width;
        static constexpr uint16_t ReportedMovementParameters_Height = DesiredMovementParameters_Height;
        static constexpr uint16_t ReportedMovementParameters_Y =
            DesiredMovementParameters_Y - ReportedMovementParameters_Height;
    };

    GFXfont const* const m_DesiredMovementParameters_Font;
    GFXfont const* const m_ReportedMovementParameters_Font;

    LCD::StaticNumericText m_Text_DesiredPosition;
    LCD::StaticNumericText m_Text_DesiredMaximumSpeed;
    LCD::StaticNumericText m_Text_DesiredMaximumAcceleration;

    LCD::StaticNumericText m_Text_ReportedPosition;
    LCD::StaticNumericText m_Text_ReportedVelocity;

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