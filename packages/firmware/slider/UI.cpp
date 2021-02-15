#include "inc/stdinc.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>

UI g_UI;

static constexpr uint16_t getEvenlyDivided(uint16_t const totalSize,
                                           uint16_t const cDivisions,
                                           uint16_t const idxDivision,
                                           uint16_t const controlSize)
{
    float const sizePerDivision = totalSize / cDivisions;
    float const controlCenter = (idxDivision + 0.5f) * sizePerDivision;
    float const controlStart = controlCenter - (controlSize * 0.5f);

    return static_cast<uint16_t>(controlStart);
}

static constexpr uint16_t getEvenlyDividedX(uint16_t const cDivisions,
                                            uint16_t const idxDivision,
                                            uint16_t const controlSize)
{
    return getEvenlyDivided(LCD::DisplayWidth, cDivisions, idxDivision, controlSize);
}

UI::UI()
    : m_FunctionColors({
          RGBColor{0xf2, 0x67, 0x39},  // Position
          RGBColor{0xf5, 0x2c, 0x68},  // Speed
          RGBColor{0xe0, 0xca, 0x3e},  // Acceleration
          RGBColor{0xff, 0xff, 0xff},  // Step
          RGBColor{0x0c, 0xf2, 0xbd},  // Rate
      })
    //
    // LCD
    //
    , m_LCD()
    , m_MovementParameterLabels_Font(&FreeSans9pt7b)
    , m_DesiredMovementParameters_Font(&FreeSans18pt7b)
    , m_ReportedMovementParameters_Font(&FreeSans12pt7b)
    // Movement program
    , m_Label_Step(m_LCD,
                   LCD::Rect{
                       X : getEvenlyDividedX(
                           LCDConstants::nMovementProgramColumns, 0, LCDConstants::MovementParameterLabels_Width),
                       Y : LCDConstants::MovementParameterLabels_Y,
                       Width : LCDConstants::MovementParameterLabels_Width,
                       Height : LCDConstants::MovementParameterLabels_Height
                   },
                   LCD::Alignment::Left,
                   m_MovementParameterLabels_Font,
                   colorFor(EncoderFunction::Step),
                   RGBColor())
    , m_Label_Position(m_LCD,
                       LCD::Rect{
                           X : getEvenlyDividedX(
                               LCDConstants::nMovementProgramColumns, 1, LCDConstants::MovementParameterLabels_Width),
                           Y : LCDConstants::MovementParameterLabels_Y,
                           Width : LCDConstants::MovementParameterLabels_Width,
                           Height : LCDConstants::MovementParameterLabels_Height
                       },
                       LCD::Alignment::Left,
                       m_MovementParameterLabels_Font,
                       colorFor(EncoderFunction::Position),
                       RGBColor())
    , m_Label_Speed(m_LCD,
                    LCD::Rect{
                        X : getEvenlyDividedX(
                            LCDConstants::nMovementProgramColumns, 2, LCDConstants::MovementParameterLabels_Width),
                        Y : LCDConstants::MovementParameterLabels_Y,
                        Width : LCDConstants::MovementParameterLabels_Width,
                        Height : LCDConstants::MovementParameterLabels_Height
                    },
                    LCD::Alignment::Left,
                    m_MovementParameterLabels_Font,
                    colorFor(EncoderFunction::Speed),
                    RGBColor())
    , m_Label_Acceleration(
          m_LCD,
          LCD::Rect{
              X : getEvenlyDividedX(
                  LCDConstants::nMovementProgramColumns, 3, LCDConstants::MovementParameterLabels_Width),
              Y : LCDConstants::MovementParameterLabels_Y,
              Width : LCDConstants::MovementParameterLabels_Width,
              Height : LCDConstants::MovementParameterLabels_Height
          },
          LCD::Alignment::Left,
          m_MovementParameterLabels_Font,
          colorFor(EncoderFunction::Acceleration),
          RGBColor())
    // Movement controls
    , m_Text_DesiredPosition(
          m_LCD,
          LCD::Rect{
              X : getEvenlyDividedX(
                  LCDConstants::nMovementControlsColumns, 0, LCDConstants::DesiredMovementParameters_Width),
              Y : LCDConstants::DesiredMovementParameters_Y,
              Width : LCDConstants::DesiredMovementParameters_Width,
              Height : LCDConstants::DesiredMovementParameters_Height
          },
          LCD::Alignment::Center,
          m_DesiredMovementParameters_Font,
          colorFor(EncoderFunction::Position),
          RGBColor(),
          LCDConstants::DesiredMovementParameters_HighlightHeight)
    , m_Text_DesiredMaximumSpeed(
          m_LCD,
          LCD::Rect{
              X : getEvenlyDividedX(
                  LCDConstants::nMovementControlsColumns, 1, LCDConstants::DesiredMovementParameters_Width),
              Y : LCDConstants::DesiredMovementParameters_Y,
              Width : LCDConstants::DesiredMovementParameters_Width,
              Height : LCDConstants::DesiredMovementParameters_Height
          },
          LCD::Alignment::Center,
          m_DesiredMovementParameters_Font,
          colorFor(EncoderFunction::Speed),
          RGBColor(),
          LCDConstants::DesiredMovementParameters_HighlightHeight)
    , m_Text_DesiredMaximumAcceleration(
          m_LCD,
          LCD::Rect{
              X : getEvenlyDividedX(
                  LCDConstants::nMovementControlsColumns, 2, LCDConstants::DesiredMovementParameters_Width),
              Y : LCDConstants::DesiredMovementParameters_Y,
              Width : LCDConstants::DesiredMovementParameters_Width,
              Height : LCDConstants::DesiredMovementParameters_Height
          },
          LCD::Alignment::Center,
          m_DesiredMovementParameters_Font,
          colorFor(EncoderFunction::Acceleration),
          RGBColor(),
          LCDConstants::DesiredMovementParameters_HighlightHeight)
    , m_Text_ReportedPosition(
          m_LCD,
          LCD::Rect{
              X : getEvenlyDividedX(
                  LCDConstants::nMovementControlsColumns, 0, LCDConstants::ReportedMovementParameters_Width),
              Y : LCDConstants::ReportedMovementParameters_Y,
              Width : LCDConstants::ReportedMovementParameters_Width,
              Height : LCDConstants::ReportedMovementParameters_Height
          },
          LCD::Alignment::Center,
          m_ReportedMovementParameters_Font,
          colorFor(EncoderFunction::Position).multiply(LCDConstants::ReportedMovementParameters_ColorMultiplier),
          RGBColor())
    , m_Text_ReportedVelocity(
          m_LCD,
          LCD::Rect{
              X : getEvenlyDividedX(
                  LCDConstants::nMovementControlsColumns, 1, LCDConstants::ReportedMovementParameters_Width),
              Y : LCDConstants::ReportedMovementParameters_Y,
              Width : LCDConstants::ReportedMovementParameters_Width,
              Height : LCDConstants::ReportedMovementParameters_Height
          },
          LCD::Alignment::Center,
          m_ReportedMovementParameters_Font,
          colorFor(EncoderFunction::Speed).multiply(LCDConstants::ReportedMovementParameters_ColorMultiplier),
          RGBColor())
    // Rate
    , m_Label_Rate(m_LCD,
                   LCD::Rect{
                       X : getEvenlyDividedX(
                           LCDConstants::nMovementControlsColumns, 2, LCDConstants::DesiredMovementParameters_Width),
                       Y : LCDConstants::DesiredRateLabel_Y,
                       Width : LCDConstants::DesiredRateLabel_Width,
                       Height : LCDConstants::DesiredRateLabel_Height
                   },
                   LCD::Alignment::Right,
                   m_MovementParameterLabels_Font,
                   colorFor(EncoderFunction::Rate),
                   RGBColor())
    , m_Text_DesiredRate(
          m_LCD,
          LCD::Rect{
              X : getEvenlyDividedX(
                  LCDConstants::nMovementControlsColumns, 2, LCDConstants::DesiredMovementParameters_Width),
              Y : LCDConstants::DesiredRateParameter_Y,
              Width : LCDConstants::DesiredRateParameter_Width,
              Height : LCDConstants::DesiredRateParameter_Height
          },
          LCD::Alignment::Right,
          m_DesiredMovementParameters_Font,
          colorFor(EncoderFunction::Rate),
          RGBColor(),
          LCDConstants::DesiredMovementParameters_HighlightHeight)

    //
    // Encoders
    //
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

    auto const setIncrementCallback = [this](EncoderFunction const encoderFunction,
                                             uint8_t const maxEncoderOrderOfMagnitude,
                                             LCD::StaticNumericText& display) {
        encoderFor(encoderFunction)
            .setPushButtonDownCallback([this, encoderFunction, maxEncoderOrderOfMagnitude, &display]() {
                uint8_t const currentOrderOfMagnitude = encoderFor(encoderFunction).getIncrementOrderOfMagnitude();
                uint8_t const updatedOrderOfMagnitude =
                    (currentOrderOfMagnitude < maxEncoderOrderOfMagnitude) ? currentOrderOfMagnitude + 1 : 0;

                encoderFor(encoderFunction).setIncrementOrderOfMagnitude(updatedOrderOfMagnitude);
                display.setActiveDigit(updatedOrderOfMagnitude);
            });
    };

    setIncrementCallback(EncoderFunction::Position, 3, m_Text_DesiredPosition);
    setIncrementCallback(EncoderFunction::Speed, 3, m_Text_DesiredMaximumSpeed);
    setIncrementCallback(EncoderFunction::Acceleration, 3, m_Text_DesiredMaximumAcceleration);
    setIncrementCallback(EncoderFunction::Rate, 2, m_Text_DesiredRate);

    // Set up observers
    g_MotorController.TargetPosition.attach(
        [this](int32_t const position) { m_Text_DesiredPosition.setValue(position); });

    g_MotorController.MaximumSpeed.attach(
        [this](int32_t const velocity) { m_Text_DesiredMaximumSpeed.setValue(velocity); });

    g_MotorController.MaximumAcceleration.attach(
        [this](uint32_t const acceleration) { m_Text_DesiredMaximumAcceleration.setValue(acceleration); });

    g_MotorController.CurrentPosition.attach(
        [this](int32_t const position) { m_Text_ReportedPosition.setValue(position); });

    g_MotorController.CurrentVelocity.attach(
        [this](int32_t const velocity) { m_Text_ReportedVelocity.setValue(velocity); });

    g_MovementProgramStore.CurrentMovementProgram.attach(
        [this](MovementProgram const& movementProgram) { m_Text_DesiredRate.setValue(movementProgram.RatePercent); });

    // Set up labels
    m_Label_Step.setText("Step");
    m_Label_Position.setText("Position");
    m_Label_Speed.setText("Speed");
    m_Label_Acceleration.setText("Acceleration");
    m_Label_Rate.setText("Rate");

    m_Text_DesiredRate.setValue(0);
}

void UI::onMainLoop()
{
    for (uint8_t idxEncoder = 0; idxEncoder < static_cast<uint8_t>(EncoderFunction::__count); ++idxEncoder)
    {
        m_Encoders[idxEncoder].pollForUpdates();
    }

    // Pick up value updates from encoders
    int32_t const positionDelta = encoderFor(EncoderFunction::Position).getLatestValueDelta();
    int32_t const speedDelta = encoderFor(EncoderFunction::Speed).getLatestValueDelta();
    int32_t const accelerationDelta = encoderFor(EncoderFunction::Acceleration).getLatestValueDelta();
    int32_t const rateDelta = encoderFor(EncoderFunction::Rate).getLatestValueDelta();

    if (positionDelta != 0)
    {
        Request request = {Type : RequestType::DesiredPositionDelta};
        request.DesiredPositionDelta.delta = positionDelta;
        g_RequestQueue.push(request);
    }

    if (speedDelta != 0)
    {
        Request request = {Type : RequestType::DesiredMaximumSpeedDelta};
        request.DesiredMaximumSpeedDelta.delta = speedDelta;
        g_RequestQueue.push(request);
    }

    if (accelerationDelta != 0)
    {
        Request request = {Type : RequestType::DesiredMaximumAccelerationDelta};
        request.DesiredMaximumAccelerationDelta.delta = accelerationDelta;
        g_RequestQueue.push(request);
    }

    if (rateDelta != 0)
    {
        MovementProgram const mutatedMovementProgram =
            g_MovementProgramStore.CurrentMovementProgram.get().mutateRate(rateDelta);

        g_MovementProgramStore.setMovementProgram(mutatedMovementProgram);
    }
}