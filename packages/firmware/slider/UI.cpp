#include "inc/stdinc.h"

UI g_UI;

UI::UI()
    : m_Wire(Wire1)  // UI hangs off (and owns) the second I2C bus
    , m_Encoders({
          Encoder(m_Wire, 0x13),
          Encoder(m_Wire, 0x14),
          Encoder(m_Wire, 0x12),
          Encoder(m_Wire, 0x11),
          Encoder(m_Wire, 0x10),
      })
{
}

void UI::begin()
{
    m_Wire.begin();

    // Reset encoders
    for (auto& encoder : m_Encoders)
    {
        encoder.begin(0);
    }

    // Give encoders time to process reset
    delay(1);

    // Finish initializing encoders
    for (auto& encoder : m_Encoders)
    {
        encoder.begin(1);
    }
}