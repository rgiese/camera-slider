#include "inc/stdinc.h"

void RGBPushButton::begin()
{
    m_ButtonInput.m_Encoder.setGPIOConfiguration(
        m_ButtonInput.m_Pin, Encoder::GPIOPinMode::DigitalInput, true /* enable pullup */);

    m_ButtonInput.m_Encoder.setGPIOEdgeCallback(m_ButtonInput.m_Pin, [this](Encoder::GPIOInputEdge const edge) {
        if (edge == Encoder::GPIOInputEdge::Negative && m_PushButtonCallback && m_fEnabled)
        {
            m_PushButtonCallback();
        }
    });

    m_Red.m_Encoder.setGPIOConfiguration(m_Red.m_Pin, Encoder::GPIOPinMode::PWMOutput);
    m_Green.m_Encoder.setGPIOConfiguration(m_Green.m_Pin, Encoder::GPIOPinMode::PWMOutput);
    m_Blue.m_Encoder.setGPIOConfiguration(m_Blue.m_Pin, Encoder::GPIOPinMode::PWMOutput);
}

void RGBPushButton::setPushButtonCallback(PushButtonCallback callback)
{
    m_PushButtonCallback = callback;
}

void RGBPushButton::setEnabled(bool const fEnabled)
{
    if (fEnabled != m_fEnabled)
    {
        m_fEnabled = fEnabled;
        updateColor();
    }
}

void RGBPushButton::setColor(RGBColor const& color, bool const fScaleColorsEvenly)
{
    // Compensate for wildly different luminosities in our RGB switch
    RGBColor const scaledColor = fScaleColorsEvenly ? color.multiply(0.3f, 0.55f, 1.0f) : color;

    if (scaledColor != m_Color)
    {
        m_Color = scaledColor;
        updateColor();
    }
}

void RGBPushButton::updateColor() const
{
    RGBColor const color = m_fEnabled ? m_Color : Colors::Black;

    m_Red.m_Encoder.setGPIOOutput(m_Red.m_Pin, color.Red);
    m_Green.m_Encoder.setGPIOOutput(m_Green.m_Pin, color.Green);
    m_Blue.m_Encoder.setGPIOOutput(m_Blue.m_Pin, color.Blue);
}