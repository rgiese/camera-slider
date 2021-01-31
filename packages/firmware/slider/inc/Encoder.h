#pragma once

class Encoder
{
public:
    Encoder(TwoWire& wire, uint8_t const address)
        : m_Wire(wire)
        , m_Address(address)
    {
    }

    void begin(uint8_t const initializationStage);

private:
    TwoWire& m_Wire;
    uint8_t const m_Address;

    //
    // Implementation constants
    // (see https://github.com/Fattoresaimon/I2CEncoderV2.1/blob/master/EncoderI2CV2.1_v1.0.pdf)
    //

    enum class I2CRegister : uint8_t
    {
        GeneralConfiguration = 0,
        GP1Configuration,
        GP2Configuration,
        GP3Configuration,
        InterruptConfiguration,
        EncoderStatus,
        InterruptStatus,
        FadeStatus,
        CounterValue_0 = 0x08,
        CounterMaxValue_0 = 0x0C,
        CounterMinValue_0 = 0x10,
        StepValue_0 = 0x14,
        LED_R = 0x18,
        LED_G,
        LED_B,
        GP1Register,
        GP2Register,
        GP3Register,
        AntiBouncingPeriod,
        DoublePushPeriod,
        FadeTimerRGB,
        FadeTimerGP,
        GeneralConfiguration2 = 0x30,
        DeviceID = 0x70,
        DeviceVersion = 0x71,
        Flash_0 = 0x80
    };

    enum class GeneralConfigurationBits : uint8_t
    {
        UseFloatInsteadOf32Bit,
        ShouldWrapCounter,
        EncoderDirection,
        DisableInterruptPullup,
        EncoderReadMode,
        EncoderIsRGB,
        EEPROMBank1,
        Reset
    };
};
