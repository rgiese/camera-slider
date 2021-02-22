#include "inc/stdinc.h"

void RGBPushButton::begin()
{
    m_Red.m_Encoder.setGPIOConfiguration(m_Red.m_Pin, Encoder::GPIOPinMode::PWMOutput);
    m_Green.m_Encoder.setGPIOConfiguration(m_Green.m_Pin, Encoder::GPIOPinMode::PWMOutput);
    m_Blue.m_Encoder.setGPIOConfiguration(m_Blue.m_Pin, Encoder::GPIOPinMode::PWMOutput);
}

void RGBPushButton::pollForUpdates()
{
}

void RGBPushButton::setColor(RGBColor const& color, bool const fScaleColorsEvenly)
{
    // Compensate for wildly different luminosities in our RGB switch
    RGBColor const scaledColor = fScaleColorsEvenly ? color.multiply(0.3f, 0.55f, 1.0f) : color;

    m_Red.m_Encoder.setGPIOOutput(m_Red.m_Pin, scaledColor.Red);
    m_Green.m_Encoder.setGPIOOutput(m_Green.m_Pin, scaledColor.Green);
    m_Blue.m_Encoder.setGPIOOutput(m_Blue.m_Pin, scaledColor.Blue);
}