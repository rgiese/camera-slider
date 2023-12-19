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
    , m_fCanControlLivePosition()
    //
    // LCD
    //
    , m_LCD()
    , m_MovementParameterLabels_Font(&FreeSans9pt7b)
    , m_DesiredMovementParameters_Font(&FreeSans18pt7b)
    , m_ReportedMovementParameters_Font(&FreeSans12pt7b)
    , m_CurrentState_Font(&FreeSans9pt7b)
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
    // Current state
    , m_Text_CurrentState(m_LCD,
                          LCD::Rect{
                              X : LCDConstants::CurrentState_X,
                              Y : LCDConstants::CurrentState_Y,
                              Width : LCDConstants::CurrentState_Width,
                              Height : LCDConstants::CurrentState_Height
                          },
                          LCD::Alignment::Center,
                          m_CurrentState_Font,
                          Colors::White.multiply(0.9f),
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
    , m_Encoders{
          Encoder(m_Wire, 0x14),  // Position
          Encoder(m_Wire, 0x13),  // Speed
          Encoder(m_Wire, 0x12),  // Acceleration
          Encoder(m_Wire, 0x10),  // Step
          Encoder(m_Wire, 0x11),  // Rate
      }
    , m_StartButton{{encoderFor(EncoderFunction::Rate), Encoder::GPIOPin::GPIO2},
                    {encoderFor(EncoderFunction::Rate), Encoder::GPIOPin::GPIO1},
                    {encoderFor(EncoderFunction::Step), Encoder::GPIOPin::GPIO1},
                    {encoderFor(EncoderFunction::Step), Encoder::GPIOPin::GPIO2}}
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
                RGBColor(),
                0,
                LCDConstants::MovementProgramTableRow_TextYOffset);
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
    //
    // Configure hardware
    //

    m_LCD.begin();

    // Configure interrupt pin for encoders
    pinMode(EncoderInterruptPin, INPUT_PULLUP);

    // Configure I2C-based encoders
    for (uint8_t idxEncoder = 0; idxEncoder < static_cast<uint8_t>(EncoderFunction::__count); ++idxEncoder)
    {
        m_Encoders[idxEncoder].begin();
        m_Encoders[idxEncoder].setColor(m_FunctionColors[idxEncoder]);
    }

    m_StartButton.begin();

    //
    // Set up static text
    //

    m_MovementProgramRows[0].Step.setText("Step");
    m_MovementProgramRows[0].DesiredPosition.setText("Position");
    m_MovementProgramRows[0].DesiredSpeed.setText("Speed");
    m_MovementProgramRows[0].DesiredAcceleration.setText("Acceleration");

    m_Label_Step.setText("Step");
    m_Label_Rate.setText("Rate");

    m_Text_CurrentState.setText("Starting");

    //
    // Configure encoder callbacks (these will run in onMainLoop->[encoders]->pollForUpdates)
    //

    // Encoder value change: movement parameters
    auto const setValueChangeCallback = [this](EncoderFunction const encoderFunction,
                                               RequestType const desiredStateRequestParameter,
                                               MovementProgram::Movement::Parameter const movementProgramParameter) {
        encoderFor(encoderFunction)
            .setValueDeltaCallback([this, desiredStateRequestParameter, movementProgramParameter](int32_t const delta) {
                if (editingExistingStep())
                {
                    // Update program
                    g_MovementProgramStore.CurrentMovementProgram.mutate(
                        [&](MovementProgram const& movementProgram) -> MovementProgram {
                            if (m_idxSelectedStep >= movementProgram.Movements.size())
                            {
                                return movementProgram;
                            }

                            MovementProgram mutatedMovementProgram = movementProgram;
                            {
                                mutatedMovementProgram.Movements[m_idxSelectedStep].applyDelta(movementProgramParameter,
                                                                                               delta);
                            }

                            return mutatedMovementProgram;
                        });

                    // Then control live position from absolute movement values
                    if (m_fCanControlLivePosition)
                    {
                        MovementProgram const& movementProgram = g_MovementProgramStore.CurrentMovementProgram;
                        movementProgram.requestMoveToMovement(m_idxSelectedStep);
                    }
                }
                else if (m_fCanControlLivePosition)
                {
                    // Control live position via delta
                    Request request = {Type : desiredStateRequestParameter};
                    request.DesiredParameterDelta.delta = delta;
                    g_RequestQueue.push(request);
                }
            });
    };

    setValueChangeCallback(EncoderFunction::Position,
                           RequestType::DesiredParameterDelta_Position,
                           MovementProgram::Movement::Parameter::DesiredPosition);
    setValueChangeCallback(EncoderFunction::Speed,
                           RequestType::DesiredParameterDelta_MaximumSpeed,
                           MovementProgram::Movement::Parameter::DesiredSpeed);
    setValueChangeCallback(EncoderFunction::Acceleration,
                           RequestType::DesiredParameterDelta_MaximumAcceleration,
                           MovementProgram::Movement::Parameter::DesiredAcceleration);

    // Encoder value change: Step
    encoderFor(EncoderFunction::Step).setValueDeltaCallback([this](int32_t delta) {
        MovementProgram const movementProgram = g_MovementProgramStore.CurrentMovementProgram;
        m_nStepsInProgram = movementProgram.Movements.size();

        uint16_t const idxSelectedStep = conformSelectedStep(delta);

        if (idxSelectedStep == m_idxSelectedStep)
        {
            // Nothing left to do
            return;
        }

        // Commit
        m_idxSelectedStep = idxSelectedStep;

        // Seek
        if (m_fCanControlLivePosition)
        {
            movementProgram.requestMoveToMovement(m_idxSelectedStep);
        }

        // Update UI
        updateWithMovementProgram(movementProgram);
    });

    // Encoder value change: Rate
    encoderFor(EncoderFunction::Rate).setValueDeltaCallback([](int32_t delta) {
        g_MovementProgramStore.CurrentMovementProgram.mutate(
            [delta](MovementProgram const& movementProgram) -> MovementProgram {
                MovementProgram mutatedMovementProgram = movementProgram;
                mutatedMovementProgram.applyRateDelta(delta);

                return mutatedMovementProgram;
            });
    });

    // Encoder push buttons for Position/Speed/Acceleration/Rate: switch 'order of magnitude' selector on push
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

    // Encoder push button for Step: add new or delete existing step
    encoderFor(EncoderFunction::Step).setPushButtonUpCallback([this](Encoder::PushDuration const pushDuration) {
        if (editingExistingStep())
        {
            // Delete
            if (pushDuration == Encoder::PushDuration::Long)
            {
                g_MovementProgramStore.CurrentMovementProgram.mutate(
                    [this](MovementProgram const& movementProgram) -> MovementProgram {
                        if (m_idxSelectedStep >= movementProgram.Movements.size())
                        {
                            return movementProgram;
                        }

                        MovementProgram mutatedMovementProgram = movementProgram;
                        mutatedMovementProgram.Movements.erase(mutatedMovementProgram.Movements.begin() +
                                                               m_idxSelectedStep);

                        return mutatedMovementProgram;
                    });
            }
        }
        else if (canAddNewStep())
        {
            // Add step
            MovementProgram::Movement movement(MovementType::Move,
                                               0,
                                               g_MotorController.TargetPosition,
                                               g_MotorController.MaximumSpeed,
                                               g_MotorController.MaximumAcceleration);

            g_MovementProgramStore.CurrentMovementProgram.mutate(
                [&movement](MovementProgram const& movementProgram) -> MovementProgram {
                    MovementProgram mutatedMovementProgram = movementProgram;
                    mutatedMovementProgram.Movements.push_back(movement);
                    return mutatedMovementProgram;
                });
        }
    });

    //
    // Configure observers
    //

    g_StateKeeper.CurrentSliderState.attach_and_initialize([this](SliderState const sliderState) {
        // Update text
        m_Text_CurrentState.setText(getSliderStateFriendlyName(sliderState));

        // Update state
        m_fCanControlLivePosition = sliderState == SliderState::TrackingDesiredPosition;
        m_idxSelectedStep = conformSelectedStep();  // Depends on previous line

        // Update control enablement
        switch (sliderState)
        {
            case SliderState::TrackingDesiredPosition:
            case SliderState::RunningMovementProgram:
                setMovementControlsEnabled(true);
                setStepAndRateControlsEnabled(true);
                break;

            default:
                setMovementControlsEnabled(false);
                setStepAndRateControlsEnabled(false);
                break;
        }

        // Update start button
        switch (sliderState)
        {
            case SliderState::TrackingDesiredPosition:
                m_StartButton.setColor(Colors::Green);
                m_StartButton.setPushButtonCallback(
                    []() { g_RequestQueue.push({Type : RequestType::StartMovementProgram}); });
                break;

            case SliderState::RunningMovementProgram:
                m_StartButton.setColor(Colors::Red);
                m_StartButton.setPushButtonCallback(
                    []() { g_RequestQueue.push({Type : RequestType::StopMovementProgram}); });
                break;

            case SliderState::CalibrateTouchStart:
            case SliderState::CalibrateTouchTopLeft:
            case SliderState::CalibrateTouchTopRight:
            case SliderState::CalibrateTouchBottomLeft:
            case SliderState::CalibrateTouchBottomRight:
                m_StartButton.setColor(Colors::Blue);
                m_StartButton.setEnabled(true);
                m_StartButton.setPushButtonCallback(
                    []() { g_RequestQueue.push({Type : RequestType::CalibrateTouchNext}); });
                break;

            default:
                m_StartButton.setColor(Colors::Black);
                m_StartButton.setPushButtonCallback([]() {});
                break;
        }
    });

    // Bind Position/Speed/Acceleration to target values iff we're not editing a step
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

    // Bind Reported Position/Speed to actual values (always)
    g_MotorController.CurrentPosition.attach_and_initialize(
        [this](int32_t const position) { m_Text_ReportedPosition.setValue(position); });

    g_MotorController.CurrentVelocity.attach_and_initialize(
        [this](int32_t const velocity) { m_Text_ReportedVelocity.setValue(velocity); });

    // Bind movement program traits to movement program updates
    g_MovementProgramStore.CurrentMovementProgram.attach_and_initialize(
        [this](MovementProgram const& movementProgram) { updateWithMovementProgram(movementProgram); });

    //
    // Initial data update
    //

    updateWithMovementProgram(g_MovementProgramStore.CurrentMovementProgram);
}

bool UI::canAddNewStep() const
{
    return m_fCanControlLivePosition;
}

uint16_t UI::conformSelectedStep(int32_t delta) const
{
    uint16_t const nStepsInProgram = m_nStepsInProgram;

    // We're using m_idxSelectedStep = m_nStepsInProgram as our sentinel "new" step.
    // However, we'll only allow the sentinel "new" step if we're actually allowed to add a new step.
    uint16_t const nAllowedSentinelSteps = canAddNewStep() ? 1 : 0;

    uint16_t nTotalAllowedSteps = nStepsInProgram + nAllowedSentinelSteps;
    {
        if (nTotalAllowedSteps > 0)
        {
            // Note: clamp_delta accepts values up to and _including_ the maximum value provided.
            --nTotalAllowedSteps;
        }
    }

    return clamp_delta<uint16_t>(m_idxSelectedStep, delta, 0, nTotalAllowedSteps);
}

bool UI::editingExistingStep() const
{
    // See note in conformSelectedStep().
    return m_idxSelectedStep < m_nStepsInProgram;
}

void UI::MovementProgramRow::updateWithMovement(uint16_t const idxMovement,
                                                MovementProgram const& movementProgram,
                                                uint16_t const idxSelectedMovement)
{
    // Update content
    if (idxMovement < movementProgram.Movements.size())
    {
        MovementProgram::Movement const& movement = movementProgram.Movements[idxMovement];

        Step.setValue(idxMovement + 1 /* human-readable */);
        DesiredPosition.setValue(movement.DesiredPosition);
        DesiredSpeed.setValue(movement.DesiredSpeed);
        DesiredAcceleration.setValue(movement.DesiredAcceleration);
    }
    else
    {
        Step.clear();
        DesiredPosition.clear();
        DesiredSpeed.clear();
        DesiredAcceleration.clear();
    }

    // Update color
    bool const fIsSelectedRow = idxMovement == idxSelectedMovement;
    RGBColor const backgroundColor = fIsSelectedRow ? RGBColor{0xFF, 0xFF, 0xFF}.multiply(0.4f) : RGBColor();

    Step.setBackgroundColor(backgroundColor);
    DesiredPosition.setBackgroundColor(backgroundColor);
    DesiredSpeed.setBackgroundColor(backgroundColor);
    DesiredAcceleration.setBackgroundColor(backgroundColor);
}

void UI::onMainLoop()
{
    // Poll encoders for updates (will deliver callbacks)
    for (uint8_t idxEncoder = 0; idxEncoder < static_cast<uint8_t>(EncoderFunction::__count); ++idxEncoder)
    {
        m_Encoders[idxEncoder].pollForUpdates();
    }
}

void UI::updateWithMovementProgram(MovementProgram const& movementProgram)
{
    //
    // Conform step state
    //

    m_nStepsInProgram = movementProgram.Movements.size();
    m_idxSelectedStep = conformSelectedStep();

    //
    // Update Step control
    //

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

    //
    // Update Rate control
    //

    m_Text_DesiredRate.setValue(movementProgram.RatePercent);

    //
    // Update movement program table
    //

    size_t const cMovementTableHeaderRows = 1;

    if (cMovementTableHeaderRows >= m_MovementProgramRows.size())
    {
        // Configuration problem
        return;
    }

    size_t const cMovementTableRows = m_MovementProgramRows.size() - cMovementTableHeaderRows;

    size_t const idxFirstMovement =  // Ensure the currently selected row is always visible
        (m_idxSelectedStep > (cMovementTableRows - 1)) ? m_idxSelectedStep - (cMovementTableRows - 1) : 0;

    for (size_t idxRow = 0; idxRow < cMovementTableRows; ++idxRow)
    {
        m_MovementProgramRows[idxRow + cMovementTableHeaderRows].updateWithMovement(
            idxRow + idxFirstMovement, movementProgram, m_idxSelectedStep);
    }

    //
    // Update Start button
    //

    m_StartButton.setEnabled(m_nStepsInProgram > 1);
}

void UI::setMovementControlsEnabled(bool const fEnabled)
{
    encoderFor(EncoderFunction::Position).setEnabled(fEnabled);
    encoderFor(EncoderFunction::Speed).setEnabled(fEnabled);
    encoderFor(EncoderFunction::Acceleration).setEnabled(fEnabled);

    m_Text_DesiredPosition.setEnabled(fEnabled);
    m_Text_DesiredMaximumSpeed.setEnabled(fEnabled);
    m_Text_DesiredMaximumAcceleration.setEnabled(fEnabled);
}

void UI::setStepAndRateControlsEnabled(bool const fEnabled)
{
    encoderFor(EncoderFunction::Step).setEnabled(fEnabled);
    encoderFor(EncoderFunction::Rate).setEnabled(fEnabled);

    m_Label_Step.setEnabled(fEnabled);
    m_Text_DesiredStep.setEnabled(fEnabled);

    m_Label_Rate.setEnabled(fEnabled);
    m_Text_DesiredRate.setEnabled(fEnabled);
}
