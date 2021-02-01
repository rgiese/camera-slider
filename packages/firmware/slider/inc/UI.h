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
    // LCD
    //

    LCD m_LCD;

private:
    //
    // Encoders
    //

    TwoWire& m_Wire;

    enum class EncoderFunction : uint8_t
    {
        Position,
        Speed,
        Acceleration,
        Step,
        Rate,
        __count
    };

    std::array<Encoder, static_cast<uint8_t>(EncoderFunction::__count)> m_Encoders;
    std::array<Encoder::Color, static_cast<uint8_t>(EncoderFunction::__count)> m_FunctionColors;

    static constexpr pin_t EncoderInterruptPin = D4;

    Encoder& encoder(EncoderFunction const encoderFunction)
    {
        return m_Encoders[static_cast<uint8_t>(encoderFunction)];
    };
};

extern UI g_UI;