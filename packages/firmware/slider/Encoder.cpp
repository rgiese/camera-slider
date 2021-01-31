#include "inc/stdinc.h"

void Encoder::begin(uint8_t const initializationStage)
{
    constexpr auto DefaultGeneralConfiguration = BitFlag<GeneralConfigurationBits>(
        GeneralConfigurationBits::DisableInterruptPullup, GeneralConfigurationBits::EncoderIsRGB);

    constexpr auto ResettingGeneralConfiguration =
        BitFlag<GeneralConfigurationBits>(GeneralConfigurationBits::Reset, DefaultGeneralConfiguration);

    m_Wire.beginTransmission(m_Address);

    switch (initializationStage)
    {
        case 0:
            // Initiate reset in GeneralConfiguration
            m_Wire.write(static_cast<uint8_t>(I2CRegister::GeneralConfiguration));
            m_Wire.write(ResettingGeneralConfiguration);
            break;

        case 1:
            // End reset in GeneralConfiguration
            m_Wire.write(static_cast<uint8_t>(I2CRegister::GeneralConfiguration));
            m_Wire.write(DefaultGeneralConfiguration);
            break;
    };

    m_Wire.endTransmission();
}