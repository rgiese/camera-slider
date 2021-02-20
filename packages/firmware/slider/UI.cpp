#include "inc/stdinc.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
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
    , m_idxSelectedStep()
    , m_nStepsInProgram()
    //
    // LCD
    //
    , m_LCD()
    , m_MovementParameterLabels_Font(&FreeSans9pt7b)
    , m_DesiredMovementParameters_Font(&FreeSans18pt7b)
    , m_ReportedMovementParameters_Font(&FreeSans12pt7b)
    // Movement controls
    , m_Text_DesiredPosition(m_LCD,
                             LCD::Rect{
                                 X : LCDConstants::DesiredMovementParameters_X(0),
                                 Y : LCDConstants::DesiredMovementParameters_Y,
                                 Width : LCDConstants::DesiredMovementParameters_Width,
                                 Height : LCDConstants::DesiredMovementParameters_Height
                             },
                             LCD::Alignment::Center,
                             m_DesiredMovementParameters_Font,
                             colorFor(EncoderFunction::Position),
                             RGBColor(),
                             LCDConstants::DesiredMovementParameters_HighlightHeight)
    , m_Text_DesiredMaximumSpeed(m_LCD,
                                 LCD::Rect{
                                     X : LCDConstants::DesiredMovementParameters_X(1),
                                     Y : LCDConstants::DesiredMovementParameters_Y,
                                     Width : LCDConstants::DesiredMovementParameters_Width,
                                     Height : LCDConstants::DesiredMovementParameters_Height
                                 },
                                 LCD::Alignment::Center,
                                 m_DesiredMovementParameters_Font,
                                 colorFor(EncoderFunction::Speed),
                                 RGBColor(),
                                 LCDConstants::DesiredMovementParameters_HighlightHeight)
    , m_Text_DesiredMaximumAcceleration(m_LCD,
                                        LCD::Rect{
                                            X : LCDConstants::DesiredMovementParameters_X(2),
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
              X : LCDConstants::DesiredMovementParameters_X(0),
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
              X : LCDConstants::DesiredMovementParameters_X(1),
              Y : LCDConstants::ReportedMovementParameters_Y,
              Width : LCDConstants::ReportedMovementParameters_Width,
              Height : LCDConstants::ReportedMovementParameters_Height
          },
          LCD::Alignment::Center,
          m_ReportedMovementParameters_Font,
          colorFor(EncoderFunction::Speed).multiply(LCDConstants::ReportedMovementParameters_ColorMultiplier),
          RGBColor())
    // Step
    , m_Label_Step(m_LCD,
                   LCD::Rect{
                       X : LCDConstants::DesiredStepAndRateLabel_X,
                       Y : LCDConstants::DesiredStepLabel_Y,
                       Width : LCDConstants::DesiredStepAndRateLabel_Width,
                       Height : LCDConstants::DesiredStepAndRateLabel_Height
                   },
                   LCD::Alignment::Right,
                   m_MovementParameterLabels_Font,
                   colorFor(EncoderFunction::Step),
                   RGBColor())
    , m_Text_DesiredStep(m_LCD,
                         LCD::Rect{
                             X : LCDConstants::DesiredStepAndRateParameter_X,
                             Y : LCDConstants::DesiredStepParameter_Y,
                             Width : LCDConstants::DesiredStepAndRateParameter_Width,
                             Height : LCDConstants::DesiredStepAndRateParameter_Height
                         },
                         LCD::Alignment::Right,
                         m_DesiredMovementParameters_Font,
                         colorFor(EncoderFunction::Step),
                         RGBColor())
    // Rate
    , m_Label_Rate(m_LCD,
                   LCD::Rect{
                       X : LCDConstants::DesiredStepAndRateLabel_X,
                       Y : LCDConstants::DesiredRateLabel_Y,
                       Width : LCDConstants::DesiredStepAndRateLabel_Width,
                       Height : LCDConstants::DesiredStepAndRateLabel_Height
                   },
                   LCD::Alignment::Right,
                   m_MovementParameterLabels_Font,
                   colorFor(EncoderFunction::Rate),
                   RGBColor())
    , m_Text_DesiredRate(m_LCD,
                         LCD::Rect{
                             X : LCDConstants::DesiredStepAndRateParameter_X,
                             Y : LCDConstants::DesiredRateParameter_Y,
                             Width : LCDConstants::DesiredStepAndRateParameter_Width,
                             Height : LCDConstants::DesiredStepAndRateParameter_Height
                         },
                         LCD::Alignment::Right,
                         m_DesiredMovementParameters_Font,
                         colorFor(EncoderFunction::Rate),
                         RGBColor(),
                         LCDConstants::DesiredMovementParameters_HighlightHeight)
    , m_MovementProgramRows()
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
    m_MovementProgramRows.reserve(LCDConstants::nMovementProgramTableRows);

    // Generate controls for entire movement program table
    auto const MovementProgramTableRow_CellX = [](uint16_t const idxColumn) -> uint16_t {
        return LCDConstants::MovementProgramTableX +
               ((idxColumn > 0) ? LCDConstants::MovementProgramTableRow_StepWidth +
                                      (idxColumn - 1) * LCDConstants::MovementProgramTableRow_MovementParameterWidth
                                : 0);
    };

    auto const MovementProgramTableRow_CellY = [](uint16_t const idxRow) -> uint16_t {
        return LCDConstants::MovementProgramTableY + idxRow * LCDConstants::MovementProgramTableRow_Height;
    };

    for (uint16_t idxRow = 0; idxRow < LCDConstants::nMovementProgramTableRows; ++idxRow)
    {
        auto const buildControl = [&](uint16_t idxColumn, RGBColor const& color) -> LCD::StaticNumericText {
            return LCD::StaticNumericText(
                m_LCD,
                LCD::Rect{MovementProgramTableRow_CellX(idxColumn),
                          MovementProgramTableRow_CellY(idxRow),
                          (idxColumn > 0) ? LCDConstants::MovementProgramTableRow_MovementParameterWidth
                                          : LCDConstants::MovementProgramTableRow_StepWidth,
                          LCDConstants::MovementProgramTableRow_Height},
                LCD::Alignment::Left,
                m_MovementParameterLabels_Font,
                color,
                RGBColor());
        };

        m_MovementProgramRows.push_back({
            buildControl(0, colorFor(EncoderFunction::Step).multiply(0.8f)),
            buildControl(1, colorFor(EncoderFunction::Position)),
            buildControl(2, colorFor(EncoderFunction::Speed)),
            buildControl(3, colorFor(EncoderFunction::Acceleration)),
        });
    }
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
    g_MotorController.TargetPosition.attach_and_initialize([this](int32_t const position) {
        if (!editingExistingStep())
        {
            m_Text_DesiredPosition.setValue(position);
        }
    });

    g_MotorController.MaximumSpeed.attach_and_initialize([this](int32_t const velocity) {
        if (!editingExistingStep())
        {
            m_Text_DesiredMaximumSpeed.setValue(velocity);
        }
    });

    g_MotorController.MaximumAcceleration.attach_and_initialize([this](uint32_t const acceleration) {
        if (!editingExistingStep())
        {
            m_Text_DesiredMaximumAcceleration.setValue(acceleration);
        }
    });

    g_MotorController.CurrentPosition.attach_and_initialize(
        [this](int32_t const position) { m_Text_ReportedPosition.setValue(position); });

    g_MotorController.CurrentVelocity.attach_and_initialize(
        [this](int32_t const velocity) { m_Text_ReportedVelocity.setValue(velocity); });

    g_MovementProgramStore.CurrentMovementProgram.attach_and_initialize([this](MovementProgram const& movementProgram) {
        // Steps and controls
        updateSelectedStep(movementProgram);

        // Rate
        m_Text_DesiredRate.setValue(movementProgram.RatePercent);

        // Movements
        size_t idxMovement = 0;

        // FUTURE: Make sure selected step is in view range
        for (; idxMovement < std::min(movementProgram.Movements.size(), m_MovementProgramRows.size() - 1);
             ++idxMovement)
        {
            // m_MovementProgramRows[0] is a header row, skip
            m_MovementProgramRows[idxMovement + 1].update(idxMovement, movementProgram.Movements[idxMovement]);
        }

        for (; idxMovement < m_MovementProgramRows.size() - 1; ++idxMovement)
        {
            m_MovementProgramRows[idxMovement + 1].clear();
        }
    });

    // Set up labels
    m_MovementProgramRows[0].Step.setText("Step");
    m_MovementProgramRows[0].DesiredPosition.setText("Position");
    m_MovementProgramRows[0].DesiredSpeed.setText("Speed");
    m_MovementProgramRows[0].DesiredAcceleration.setText("Acceleration");

    m_Label_Step.setText("Step");
    m_Label_Rate.setText("Rate");

    updateSelectedStep(g_MovementProgramStore.CurrentMovementProgram);
}

void UI::MovementProgramRow::clear()
{
    Step.clear();
    DesiredPosition.clear();
    DesiredSpeed.clear();
    DesiredAcceleration.clear();
}

void UI::MovementProgramRow::update(uint16_t const idxMovement, MovementProgram::Movement const& movement)
{
    Step.setValue(idxMovement + 1 /* human-readable */);
    DesiredPosition.setValue(movement.DesiredPosition);
    DesiredSpeed.setValue(movement.DesiredSpeed);
    DesiredAcceleration.setValue(movement.DesiredAcceleration);
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
    int32_t const stepDelta = encoderFor(EncoderFunction::Step).getLatestValueDelta();
    int32_t const rateDelta = encoderFor(EncoderFunction::Rate).getLatestValueDelta();

    // Apply movement parameters
    if (editingExistingStep())
    {
        // Editing existing step - update program
    }
    else
    {
        // Creating new step - control live position
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
    }

    // Apply step selection
    if (stepDelta != 0)
    {
        updateSelectedStep(g_MovementProgramStore.CurrentMovementProgram, stepDelta);
    }

    // Apply rate selection
    if (rateDelta != 0)
    {
        g_MovementProgramStore.CurrentMovementProgram.mutate(
            [rateDelta](MovementProgram const& movementProgram) -> MovementProgram {
                MovementProgram mutatedMovementProgram = movementProgram;
                mutatedMovementProgram.applyRateDelta(rateDelta);

                return mutatedMovementProgram;
            });
    }
}

void UI::updateSelectedStep(MovementProgram const& movementProgram, int16_t const idxSelectedStepDelta)
{
    m_nStepsInProgram = movementProgram.Movements.size();

    // Note: clamp_delta accepts values up to and _including_ the maximum value provided (m_nStepsInProgram)
    // This is desired because we're using m_idxSelectedStep = m_nStepsInProgram as our sentinel "new" step.
    m_idxSelectedStep = clamp_delta<uint16_t>(m_idxSelectedStep, idxSelectedStepDelta, 0, m_nStepsInProgram);

    if (editingExistingStep())
    {
        m_Text_DesiredStep.setValue(m_idxSelectedStep + 1 /* human-readable */);

        MovementProgram::Movement const& movement = movementProgram.Movements[m_idxSelectedStep];

        m_Text_DesiredPosition.setValue(movement.DesiredPosition);
        m_Text_DesiredMaximumSpeed.setValue(movement.DesiredSpeed);
        m_Text_DesiredMaximumAcceleration.setValue(movement.DesiredAcceleration);
    }
    else
    {
        m_Text_DesiredStep.setText("new");

        m_Text_DesiredPosition.setValue(g_MotorController.TargetPosition);
        m_Text_DesiredMaximumSpeed.setValue(g_MotorController.MaximumSpeed);
        m_Text_DesiredMaximumAcceleration.setValue(g_MotorController.MaximumAcceleration);
    }
}

bool UI::editingExistingStep() const
{
    return m_idxSelectedStep < m_nStepsInProgram;
}
