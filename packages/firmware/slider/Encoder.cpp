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
}

void Encoder::setColor(RGBColor const& color)
{
    AutoTransmission autoTransmission(m_Wire, m_Address);

    m_Wire.write(static_cast<uint8_t>(I2CRegister::LED_R));
    m_Wire.write(color.Red);
    m_Wire.write(color.Green);
    m_Wire.write(color.Blue);
}

void Encoder::setIncrementValue(uint32_t const incrementValue)
{
    writeRegister(I2CRegister::StepValue_0, incrementValue);
}

int32_t Encoder::getLatestValueDelta()
{
    return readRegister<int32_t>(I2CRegister::CounterValue_0);
}
