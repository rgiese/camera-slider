#include "inc/stdinc.h"

#include <Fonts/FreeSans18pt7b.h>

UI g_UI;

UI::UI()
    : m_FunctionColors({
          RGBColor{0xf2, 0x67, 0x39},  // Position
          RGBColor{0xf5, 0x2c, 0x68},  // Speed
          RGBColor{0xe0, 0xca, 0x3e},  // Acceleration
          RGBColor{0xff, 0xff, 0xff},  // Step
          RGBColor{0x0c, 0xf2, 0xbd},  // Rate
      })
    // LCD
    , m_LCD()
    , m_PositionSpeedAcceleration_Font(&FreeSans18pt7b)
    , m_PositionText(m_LCD,
                     LCD::Rect{
                         X : 0,
                         Y : LCDConstants::PositionSpeedAcceleration_Y,
                         Width : LCDConstants::PositionSpeedAcceleration_Width,
                         Height : LCDConstants::PositionSpeedAcceleration_Height
                     },
                     LCD::Alignment::Center,
                     m_PositionSpeedAcceleration_Font,
                     colorFor(EncoderFunction::Position),
                     RGBColor())
    , m_SpeedText(m_LCD,
                  LCD::Rect{
                      X : (LCD::DisplayWidth - LCDConstants::PositionSpeedAcceleration_Width) / 2,
                      Y : LCDConstants::PositionSpeedAcceleration_Y,
                      Width : LCDConstants::PositionSpeedAcceleration_Width,
                      Height : LCDConstants::PositionSpeedAcceleration_Height
                  },
                  LCD::Alignment::Center,
                  m_PositionSpeedAcceleration_Font,
                  colorFor(EncoderFunction::Speed),
                  RGBColor())
    , m_AccelerationText(m_LCD,
                         LCD::Rect{
                             X : LCD::DisplayWidth - LCDConstants::PositionSpeedAcceleration_Width,
                             Y : LCDConstants::PositionSpeedAcceleration_Y,
                             Width : LCDConstants::PositionSpeedAcceleration_Width,
                             Height : LCDConstants::PositionSpeedAcceleration_Height
                         },
                         LCD::Alignment::Center,
                         m_PositionSpeedAcceleration_Font,
                         colorFor(EncoderFunction::Acceleration),
                         RGBColor())
    // Encoders
    , m_Wire(Wire1)  // UI hangs off (and owns) the second I2C bus
    , m_Encoders({
          Encoder(m_Wire, 0x14),  // Position
          Encoder(m_Wire, 0x13),  // Speed
          Encoder(m_Wire, 0x12),  // Acceleration
          Encoder(m_Wire, 0x10),  // Step
          Encoder(m_Wire, 0x11),  // Rate
      })
{
}

void UI::begin()
{
    m_LCD.begin();

    // Configure interrupt pin for encoders
    pinMode(EncoderInterruptPin, INPUT_PULLUP);

    // Configure I2C-based encoders
    for (uint8_t idxEncoder = 0; idxEncoder < static_cast<uint8_t>(EncoderFunction::__count); ++idxEncoder)
    {
        m_Encoders[idxEncoder].begin();
        m_Encoders[idxEncoder].setColor(m_FunctionColors[idxEncoder]);
    }

    // Set up observers
    g_MotorController.CurrentPosition.attach(
        [this](int32_t const position) { m_PositionText.setText(std::to_string(position)); });

    g_MotorController.CurrentVelocity.attach(
        [this](int32_t const velocity) { m_SpeedText.setText(std::to_string(velocity)); });

    g_MotorController.MaximumAcceleration.attach(
        [this](uint32_t const acceleration) { m_AccelerationText.setText(std::to_string(acceleration)); });
}