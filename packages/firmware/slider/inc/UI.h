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
};

extern UI g_UI;