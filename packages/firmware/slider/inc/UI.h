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
        Position,
        Speed,
        Acceleration,
        Step,
        Rate,
        __count
    };

    RGBColor m_FunctionColors[static_cast<size_t>(EncoderFunction::__count)];

    constexpr RGBColor const& colorFor(EncoderFunction const encoderFunction)
    {
        return m_FunctionColors[static_cast<uint8_t>(encoderFunction)];
    };

private:
    //
    // LCD
    //

    LCD m_LCD;

    struct LCDConstants
    {
        static constexpr uint16_t PaddingSmall = 4;
        static constexpr uint16_t PaddingMedium = 6;

        // Movement program table
        static constexpr uint16_t MovementProgramTableX = PaddingSmall;
        static constexpr uint16_t MovementProgramTableY = PaddingSmall;

        static constexpr uint16_t MovementProgramTableRow_StepWidth = 50;
        static constexpr uint16_t MovementProgramTableRow_MovementParameterWidth = 100;
        static constexpr uint16_t MovementProgramTableRow_Height = 18;

        static constexpr uint16_t nMovementProgramTableRows = 7;

        // Movement controls row
        static constexpr uint16_t nMovementControlsColumns = 3;

        static constexpr uint16_t DesiredMovementParameters_Width =
            std::min(128, LCD::DisplayWidth / nMovementControlsColumns);
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

        // Rate control
        static constexpr uint16_t DesiredRateParameter_Width = DesiredMovementParameters_Width;
        static constexpr uint16_t DesiredRateParameter_Height = DesiredMovementParameters_Height;
        static constexpr uint16_t DesiredRateParameter_X =
            LCD::DisplayWidth - DesiredRateParameter_Width - PaddingSmall;
        static constexpr uint16_t DesiredRateParameter_Y =
            UITools::getEvenlyDivided(LCD::DisplayHeight, 2, 1, DesiredRateParameter_Height);

        static constexpr uint16_t DesiredRateLabel_Width = DesiredRateParameter_Width;
        static constexpr uint16_t DesiredRateLabel_Height = 18;
        static constexpr uint16_t DesiredRateLabel_X = DesiredRateParameter_X;
        static constexpr uint16_t DesiredRateLabel_Y = DesiredRateParameter_Y - DesiredRateLabel_Height;
    };

    GFXfont const* const m_MovementParameterLabels_Font;
    GFXfont const* const m_DesiredMovementParameters_Font;
    GFXfont const* const m_ReportedMovementParameters_Font;

    LCD::StaticNumericText m_Text_DesiredPosition;
    LCD::StaticNumericText m_Text_DesiredMaximumSpeed;
    LCD::StaticNumericText m_Text_DesiredMaximumAcceleration;

    LCD::StaticNumericText m_Text_ReportedPosition;
    LCD::StaticNumericText m_Text_ReportedVelocity;

    LCD::StaticText m_Label_Rate;
    LCD::StaticNumericText m_Text_DesiredRate;

    struct MovementProgramRow
    {
        LCD::StaticNumericText Step;
        LCD::StaticNumericText DesiredPosition;
        LCD::StaticNumericText DesiredSpeed;
        LCD::StaticNumericText DesiredAcceleration;

        void clear();
        void update(uint16_t const idxMovement, MovementProgram::Movement const& movement);
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
};

extern UI g_UI;