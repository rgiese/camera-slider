#include "inc/stdinc.h"

void Encoder::begin()
{
    // Initiate reset in GeneralConfiguration
    {
        AutoTransmission autoTransmission(m_Wire, m_Address);

        m_Wire.write(static_cast<uint8_t>(I2CRegister::GeneralConfiguration));
        m_Wire.write(GeneralConfigurationRegister{.Reset = true});
    }

    // Wait for reset to complete
    delay(1);

    // Set up configuration:
    // - Don't need pullups, is RGB
    // - Operate in relative mode (device will automatically clear current value register on every read operation)
    // - Set range to maximum signed range
    writeRegister(I2CRegister::GeneralConfiguration,
                  GeneralConfigurationRegister{.DisableInterruptPullup = true, .EncoderIsRGB = true});

    writeRegister(I2CRegister::GeneralConfiguration2, GeneralConfiguration2Register{.RelativeMode = true});

    writeRegister(I2CRegister::CounterMaxValue_0, std::numeric_limits<int32_t>::max());
    writeRegister(I2CRegister::CounterMinValue_0, std::numeric_limits<int32_t>::min());

    setIncrementOrderOfMagnitude(0);
}

void Encoder::setPushButtonDownCallback(PushButtonCallback callback)
{
    m_PushButtonDownCallback = callback;
}

void Encoder::pollForUpdates()
{
    EncoderStatusBits encoderStatus{0};
    {
        encoderStatus._Value = readRegister<uint8_t>(I2CRegister::EncoderStatus);
    }

    if (encoderStatus.IsPushButtonPressed && m_PushButtonDownCallback)
    {
        m_PushButtonDownCallback();
    }
}

void Encoder::setColor(RGBColor const& color)
{
    AutoTransmission autoTransmission(m_Wire, m_Address);

    m_Wire.write(static_cast<uint8_t>(I2CRegister::LED_R));
    m_Wire.write(color.Red);
    m_Wire.write(color.Green);
    m_Wire.write(color.Blue);
}

void Encoder::setIncrementOrderOfMagnitude(uint8_t const incrementOrderOfMagnitude)
{
    m_IncrementOrderOfMagnitude = incrementOrderOfMagnitude;

    auto const powerOf10 = [](uint8_t const exponent) -> uint32_t {
        uint32_t value = 1;

        for (uint8_t idxExponent = 0; idxExponent < exponent; ++idxExponent)
        {
            value = value * 10;
        }

        return value;
    };

    writeRegister(I2CRegister::StepValue_0, powerOf10(m_IncrementOrderOfMagnitude));
}

uint8_t Encoder::getIncrementOrderOfMagnitude()
{
    return m_IncrementOrderOfMagnitude;
}

int32_t Encoder::getLatestValueDelta()
{
    return readRegister<int32_t>(I2CRegister::CounterValue_0);
}
