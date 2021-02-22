#pragma once

class RGBPushButton
{
public:
    struct EncoderAndGPIOPin
    {
        Encoder& m_Encoder;
        Encoder::GPIOPin m_Pin;
    };

public:
    RGBPushButton(EncoderAndGPIOPin buttonInput,
                  EncoderAndGPIOPin ledRed,
                  EncoderAndGPIOPin ledGreen,
                  EncoderAndGPIOPin ledBlue)
        : m_ButtonInput(buttonInput)
        , m_Red(ledRed)
        , m_Green(ledGreen)
        , m_Blue(ledBlue)
    {
    }

public:
    //
    // Initialization
    //
    void begin();

public:
    //
    // Runtime
    //
    void pollForUpdates();

    void setColor(RGBColor const& color, bool const fScaleColorsEvenly = true);

private:
    // Non-copyable
    RGBPushButton(RGBPushButton const&) = delete;
    RGBPushButton& operator=(RGBPushButton const&) = delete;

private:
    EncoderAndGPIOPin const m_ButtonInput;
    EncoderAndGPIOPin const m_Red;
    EncoderAndGPIOPin const m_Green;
    EncoderAndGPIOPin const m_Blue;
};