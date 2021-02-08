#pragma once

class Encoder
{
public:
    Encoder(TwoWire& wire, uint8_t const address)
        : m_Wire(wire)
        , m_Address(address)
    {
    }

    void begin();

    void setColor(RGBColor const& color);

    void setIncrementValue(uint32_t const incrementValue);
    int32_t getLatestValueDelta();

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

    struct GeneralConfigurationRegister
    {
        bool UseFloatInsteadOf32Bit : 1;
        bool ShouldWrapCounter : 1;
        bool EncoderDirection : 1;
        bool DisableInterruptPullup : 1;
        bool EncoderReadMode : 1;
        bool EncoderIsRGB : 1;
        bool EEPROMBank1 : 1;
        bool Reset : 1;

        operator uint8_t() const
        {
            static_assert(sizeof(*this) == sizeof(uint8_t));
            return *reinterpret_cast<uint8_t const*>(this);
        };
    };

    struct GeneralConfiguration2Register
    {
        bool EnableI2CClockStretch : 1;
        bool RelativeMode : 1;
        int8_t Unused : 6;

        operator uint8_t() const
        {
            static_assert(sizeof(*this) == sizeof(uint8_t));
            return *reinterpret_cast<uint8_t const*>(this);
        };
    };

    struct InterruptConfigurationRegister
    {
        bool PushButtonRelease : 1;
        bool PushButtonPress : 1;
        bool PushButtonDoublePress : 1;
        bool RotaryIncrease : 1;
        bool RotaryDecrease : 1;
        bool MaximumValueReached : 1;
        bool MinimumValueReached : 1;
        bool SecondaryInterrupts : 1;

        operator uint8_t() const
        {
            static_assert(sizeof(*this) == sizeof(uint8_t));
            return *reinterpret_cast<uint8_t const*>(this);
        };
    };

    struct EncoderStatusBits
    {
        union
        {
            struct
            {
                bool IsPushButtonReleased : 1;
                bool IsPushButtonPressed : 1;
                bool IsPushButtonDoublePressed : 1;
                bool IsEncoderIncreased : 1;
                bool IsEncoderDecreased : 1;
                bool IsValueAtMaximum : 1;
                bool IsValueAtMinimum : 1;
                bool HasSecondaryInterrupt : 1;
            };
            uint8_t _Value;
        };

        EncoderStatusBits(uint8_t const value)
            : _Value(value)
        {
            static_assert(sizeof(*this) == sizeof(uint8_t));
        }
    };

private:
    //
    // I2C tools
    //

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

    template <typename T>
    void writeScalar(T const value) const
    {
        std::array<uint8_t, sizeof(T)> rgData;

        *reinterpret_cast<T*>(rgData.data()) = value;

        for (int8_t idxByte = sizeof(T) - 1; idxByte >= 0; --idxByte)
        {
            m_Wire.write(rgData[idxByte]);
        }
    }

    template <typename T>
    void writeRegister(I2CRegister const i2cRegister, T const value)
    {
        AutoTransmission autoTransmission(m_Wire, m_Address);
        m_Wire.write(static_cast<uint8_t>(i2cRegister));

        writeScalar(value);
    }

    template <typename T>
    T readScalar() const
    {
        std::array<uint8_t, sizeof(T)> rgData;

        for (int8_t idxByte = sizeof(T) - 1; idxByte >= 0; --idxByte)
        {
            rgData[idxByte] = m_Wire.read();
        }

        return *reinterpret_cast<T const*>(rgData.data());
    }

    template <typename T>
    T readRegister(I2CRegister const i2cRegister)
    {
        {
            AutoTransmission autoTransmission(m_Wire, m_Address);
            m_Wire.write(static_cast<uint8_t>(i2cRegister));
        }

        T value = 0;
        {
            m_Wire.requestFrom(m_Address, sizeof(value));

            if (m_Wire.available())
            {
                // Reads and auto-zeroes (.RelativeMode = true) the current value
                value = readScalar<decltype(value)>();
            }
            else
            {
                Serial.println("!! I2C read problem: not available");
            }
        }

        return value;
    }
};
