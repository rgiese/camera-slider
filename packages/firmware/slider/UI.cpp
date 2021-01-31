#include "inc/stdinc.h"

UI g_UI;

UI::UI()
    : m_Wire(Wire1)  // UI hangs off (and owns) the second I2C bus
    , m_Encoders({
          Encoder(m_Wire, 0x14),  // Position
          Encoder(m_Wire, 0x13),  // Speed
          Encoder(m_Wire, 0x12),  // Acceleration
          Encoder(m_Wire, 0x10),  // Step
          Encoder(m_Wire, 0x11),  // Rate
      })
    , m_FunctionColors({
          Encoder::Color{0xf2, 0x67, 0x39},  // Position
          Encoder::Color{0xf5, 0x2c, 0x68},  // Speed
          Encoder::Color{0xe0, 0xca, 0x3e},  // Acceleration
          Encoder::Color{0xff, 0xff, 0xff},  // Step
          Encoder::Color{0x0c, 0xf2, 0xbd},  // Rate
      })
{
}

void UI::begin()
{
    // Configure control outputs for LCD
    analogWrite(LCDBrightnessPin, 255);
    digitalWrite(LCDResetPin, HIGH);

    // Configure interrupt pin for encoders
    pinMode(EncoderInterruptPin, INPUT_PULLUP);

    // Configure I2C-based encoders
    m_Wire.setSpeed(CLOCK_SPEED_400KHZ);
    m_Wire.begin();

    for (auto& encoder : m_Encoders)
    {
        encoder.begin();
    }

    for (uint8_t idxEncoder = 0; idxEncoder < static_cast<uint8_t>(EncoderFunction::__count); ++idxEncoder)
    {
        m_Encoders[idxEncoder].setColor(m_FunctionColors[idxEncoder]);
    }
}