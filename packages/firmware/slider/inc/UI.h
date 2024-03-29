#pragma once

namespace UITools
{
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
};  // namespace UITools

class UI
{
public:
    UI();

    void begin();
    void onMainLoop();

private:
    // Non-copyable
    UI(UI const&) = delete;
    UI& operator=(UI const&) = delete;

private:
    //
    // Shared LCD/Encoder
    //

    enum class EncoderFunction : uint8_t
    {
        // Physical encoders
        Position,
        Speed,
        Acceleration,
        Step,
        Rate,
        __count,
        // Virtual encoders
        Deceleration,
    };

    enum class AccelerationEncoderMappedTo
    {
        Acceleration,
        Deceleration,
    };

    constexpr RGBColor const colorFor(EncoderFunction const encoderFunction)
    {
        switch (encoderFunction)
        {
            case EncoderFunction::Position:
                return RGBColor{0xf2, 0x67, 0x39};
            case EncoderFunction::Speed:
                return RGBColor{0xf5, 0x2c, 0x68};
            case EncoderFunction::Acceleration:
                return RGBColor{0xe0, 0xca, 0x3e};
            case EncoderFunction::Deceleration:
                return RGBColor{0x3e, 0xe0, 0x46};
            case EncoderFunction::Step:
                return RGBColor{0xff, 0xff, 0xff};
            case EncoderFunction::Rate:
                return RGBColor{0x0c, 0xf2, 0xbd};
            default:
                return Colors::Black;
        }
    };

private:
    //
    // State
    //

    uint16_t m_idxSelectedStep;
    uint16_t m_nStepsInProgram;
    bool m_fCanControlLivePosition;
    AccelerationEncoderMappedTo m_AccelerationEncoderMappedTo;

    bool canAddNewStep() const;
    uint16_t conformSelectedStep(int32_t delta = 0) const;
    bool editingExistingStep() const;

    void updateWithMovementProgram(MovementProgram const& movementProgram);

    void setMovementControlsEnabled(bool const fEnabled);
    void setStepAndRateControlsEnabled(bool const fEnabled);

    void updateAccelerationControlColor();

private:
    //
    // LCD
    //

    LCD m_LCD;

    struct LCDConstants
    {
        static constexpr uint16_t PaddingSmall = 4;
        static constexpr uint16_t PaddingMedium = 6;

        //
        // Movement program table
        //

        static constexpr uint16_t MovementProgramTableX = PaddingSmall;
        static constexpr uint16_t MovementProgramTableY = PaddingSmall;

        static constexpr uint16_t MovementProgramTableRow_StepWidth = 50;
        static constexpr uint16_t MovementProgramTableRow_MovementParameterWidth = 80;
        static constexpr uint16_t MovementProgramTableRow_Height = 18;
        static constexpr uint16_t MovementProgramTableRow_TextYOffset = 2;

        static constexpr uint16_t nMovementProgramTableRows = 7;
        static constexpr uint16_t nMovementProgramTableColumns = 4;

        // Advisory values for laying out surrounding controls
        static constexpr uint16_t MovementProgramTableWidth =
            MovementProgramTableRow_StepWidth +
            nMovementProgramTableColumns * MovementProgramTableRow_MovementParameterWidth;
        static constexpr uint16_t MovementProgramTableRightX = MovementProgramTableX + MovementProgramTableWidth;

        //
        // Movement controls row
        //

        static constexpr uint16_t nMovementControlsColumns = 3;

        static constexpr uint16_t DesiredMovementParameters_Width =
            std::min<uint16_t>(LCD::StaticText::c_MaximumWidth, LCD::DisplayWidth / nMovementControlsColumns);
        static constexpr uint16_t DesiredMovementParameters_Height = 32;
        static constexpr uint16_t DesiredMovementParameters_HighlightHeight = 2;
        static constexpr uint16_t DesiredMovementParameters_X(uint16_t const idxLabel)
        {
            return UITools::getEvenlyDivided(
                LCD::DisplayWidth, nMovementControlsColumns, idxLabel, DesiredMovementParameters_Width);
        }
        static constexpr uint16_t DesiredMovementParameters_Y =
            LCD::DisplayHeight - DesiredMovementParameters_Height - PaddingMedium;

        static constexpr uint16_t ReportedMovementParameters_Width = DesiredMovementParameters_Width;
        static constexpr uint16_t ReportedMovementParameters_Height = 24;
        static constexpr uint16_t ReportedMovementParameters_Y =
            DesiredMovementParameters_Y - ReportedMovementParameters_Height - PaddingSmall;
        static constexpr float ReportedMovementParameters_ColorMultiplier = 0.8f;

        //
        // State display row
        //

        static constexpr uint16_t CurrentState_Width = LCD::StaticText::c_MaximumWidth;
        static constexpr uint16_t CurrentState_X = (LCD::DisplayWidth - CurrentState_Width) / 2;

        static constexpr uint16_t CurrentState_Height = ReportedMovementParameters_Height;
        static constexpr uint16_t CurrentState_Y =
            ReportedMovementParameters_Y - CurrentState_Height - 2 * PaddingMedium;

        //
        // Step and rate controls column
        //

        static constexpr uint16_t DesiredStepAndRateParameter_AvailableWidth =
            LCD::DisplayWidth - MovementProgramTableRightX;

        static constexpr uint16_t DesiredStepAndRateParameter_X = MovementProgramTableRightX;
        static constexpr uint16_t DesiredStepAndRateParameter_Width =
            DesiredStepAndRateParameter_AvailableWidth - PaddingSmall;
        static constexpr uint16_t DesiredStepAndRateParameter_Height = DesiredMovementParameters_Height;

        static_assert(DesiredStepAndRateParameter_Width <= LCD::StaticText::c_MaximumWidth);

        static constexpr uint16_t DesiredStepAndRateLabel_Width = DesiredStepAndRateParameter_Width;
        static constexpr uint16_t DesiredStepAndRateLabel_Height = 18;
        static constexpr uint16_t DesiredStepAndRateLabel_X = DesiredStepAndRateParameter_X;

        // Step control
        static constexpr uint16_t DesiredStepParameter_Y =
            UITools::getEvenlyDivided(LCD::DisplayHeight, 2, 0, DesiredStepAndRateParameter_Height);

        static constexpr uint16_t DesiredStepLabel_Y = DesiredStepParameter_Y - DesiredStepAndRateLabel_Height;

        // Rate control
        static constexpr uint16_t DesiredRateParameter_Y =
            UITools::getEvenlyDivided(LCD::DisplayHeight, 2, 1, DesiredStepAndRateParameter_Height);

        static constexpr uint16_t DesiredRateLabel_Y = DesiredRateParameter_Y - DesiredStepAndRateLabel_Height;
    };

    GFXfont const* const m_MovementParameterLabels_Font;
    GFXfont const* const m_DesiredMovementParameters_Font;
    GFXfont const* const m_ReportedMovementParameters_Font;
    GFXfont const* const m_CurrentState_Font;

    LCD::StaticNumericText m_Text_DesiredPosition;
    LCD::StaticNumericText m_Text_DesiredMaximumSpeed;
    LCD::StaticNumericText m_Text_DesiredMaximumAcceleration;

    LCD::StaticNumericText m_Text_ReportedPosition;
    LCD::StaticNumericText m_Text_ReportedVelocity;

    LCD::StaticText m_Text_CurrentState;

    LCD::StaticText m_Label_Step;
    LCD::StaticNumericText m_Text_DesiredStep;

    LCD::StaticText m_Label_Rate;
    LCD::StaticNumericText m_Text_DesiredRate;

    struct MovementProgramRow
    {
        LCD::StaticNumericText Step;
        LCD::StaticNumericText DesiredPosition;
        LCD::StaticNumericText DesiredSpeed;
        LCD::StaticNumericText DesiredAcceleration;
        LCD::StaticNumericText DesiredDeceleration;

        void updateWithMovement(uint16_t const idxMovement,
                                MovementProgram const& movementProgram,
                                uint16_t const idxSelectedMovement);
    };

    std::vector<MovementProgramRow> m_MovementProgramRows;

private:
    //
    // Encoders
    //

    TwoWire& m_Wire;

    static constexpr pin_t EncoderInterruptPin = D4;

    Encoder m_Encoders[static_cast<size_t>(EncoderFunction::__count)];

    constexpr Encoder& encoderFor(EncoderFunction const encoderFunction)
    {
        return m_Encoders[static_cast<uint8_t>(encoderFunction)];
    };

    RGBPushButton m_StartButton;
};

extern UI g_UI;