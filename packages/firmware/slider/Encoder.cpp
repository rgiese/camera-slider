#include "inc/stdinc.h"

class AutoTransmission
{
public:
    AutoTransmission(TwoWire& wire, uint8_t const address)
        : m_Wire(wire)
    {
        m_Wire.beginTransmission(WireTransmission(address).timeout(100ms));
    }

    ~AutoTransmission()
    {
        uint8_t const transmissionStatus = m_Wire.endTransmission();

        if (transmissionStatus != 0)
        {
            Serial.printlnf("!! I2C transmission problem: code %u.", transmissionStatus);
        }
    }

private:
    // Non-copyable
    AutoTransmission(AutoTransmission const&) = delete;
    AutoTransmission& operator=(AutoTransmission const&) = delete;

private:
    TwoWire& m_Wire;
};

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

    // Set up configuration
    {
        AutoTransmission autoTransmission(m_Wire, m_Address);

        m_Wire.write(static_cast<uint8_t>(I2CRegister::GeneralConfiguration));
        m_Wire.write(GeneralConfigurationRegister{.DisableInterruptPullup = true, .EncoderIsRGB = true});
    }
}

void Encoder::setColor(Encoder::Color const& color)
{
    AutoTransmission autoTransmission(m_Wire, m_Address);

    m_Wire.write(static_cast<uint8_t>(I2CRegister::LED_R));
    m_Wire.write(color.Red);
    m_Wire.write(color.Green);
    m_Wire.write(color.Blue);
}
