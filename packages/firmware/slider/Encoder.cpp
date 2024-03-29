#include "inc/stdinc.h"

void Encoder::begin()
{
    // Initiate reset in GeneralConfiguration
    writeRegister(I2CRegister::GeneralConfiguration, GeneralConfigurationRegister{.Reset = true});

    // Wait for reset to complete
    delay(1);

    // Set up configuration:
    // - Don't need pullups, is RGB
    // - Operate in relative mode (device will automatically clear current value register on every read operation)
    // - Set range to maximum signed range
    writeRegister(I2CRegister::GeneralConfiguration,
                  GeneralConfigurationRegister{.DisableInterruptPullup = true, .EncoderIsRGB = true});

    writeRegister(I2CRegister::GeneralConfiguration2, GeneralConfiguration2Register{.RelativeMode = true});

    writeRegister(I2CRegister::CounterMaxValue_0, std::numeric_limits<int32_t>::max());
    writeRegister(I2CRegister::CounterMinValue_0, std::numeric_limits<int32_t>::min());

    setIncrementOrderOfMagnitude(0);
}

void Encoder::setValueDeltaCallback(ValueDeltaCallback callback)
{
    m_ValueDeltaCallback = callback;
}

void Encoder::setPushButtonDownCallback(PushButtonDownCallback callback)
{
    m_PushButtonDownCallback = callback;
}

void Encoder::setPushButtonUpCallback(PushButtonUpCallback callback)
{
    m_PushButtonUpCallback = callback;
}

void Encoder::setGPIOConfiguration(GPIOPin const pin, GPIOPinMode const pinMode, bool const enablePullup)
{
    getGPIOConfiguration(pin).PinMode = pinMode;

    GPIOConfigurationRegister configuration;
    {
        switch (pinMode)
        {
            case GPIOPinMode::PWMOutput:
                configuration.PinMode = 0b00;
                break;

            case GPIOPinMode::PushPullOutput:
                configuration.PinMode = 0b01;
                break;

            case GPIOPinMode::AnalogInput:
                configuration.PinMode = 0b10;
                break;

            case GPIOPinMode::DigitalInput:
                configuration.PinMode = 0b11;
                configuration.EnablePullup = enablePullup;
                configuration.InterruptConfiguration = 0b11;  // Interrupts on both edges
                break;
        }
    }

    writeRegister(getGPIOConfigurationRegister(pin), configuration);
}

void Encoder::setGPIOEdgeCallback(GPIOPin const pin, GPIOInputEdgeCallback callback)
{
    getGPIOConfiguration(pin).InputEdgeCallback = callback;
}

void Encoder::pollForUpdates()
{
    //
    // Read values from device (clears registers)
    //

    EncoderStatusBits encoderStatus{0};
    {
        encoderStatus._Value = readRegister<uint8_t>(I2CRegister::EncoderStatus);
    }

    int32_t latestValueDelta = 0;
    {
        if (encoderStatus.IsEncoderIncreased || encoderStatus.IsEncoderDecreased)
        {
            latestValueDelta = readRegister<int32_t>(I2CRegister::CounterValue_0);
        }
    }

    SecondaryInterruptStatusBits secondaryInterruptStatus{0};
    {
        secondaryInterruptStatus._Value = readRegister<uint8_t>(I2CRegister::SecondaryInterruptStatus);
    }

    if (m_fEnabled)
    {
        // Evaluate value change
        if (latestValueDelta && m_ValueDeltaCallback)
        {
            m_ValueDeltaCallback(latestValueDelta);
        }

        // Evaluate pressed -> released in order in case we got a down+up in a single poll cycle
        if (encoderStatus.IsPushButtonPressed)
        {
            m_TimePushButtonPressedDown = millis();

            if (m_PushButtonDownCallback)
            {
                m_PushButtonDownCallback();
            }
        }

        if (m_TimePushButtonPressedDown /* as a proxy for whether we're in a push right now */)
        {
            unsigned long const currentTime = millis();
            unsigned long const durationPressed_msec = currentTime - m_TimePushButtonPressedDown;

            if (encoderStatus.IsPushButtonReleased || (durationPressed_msec >= c_LongPushThreshold_msec))
            {
                if (m_PushButtonUpCallback)
                {
                    m_PushButtonUpCallback(durationPressed_msec >= c_LongPushThreshold_msec ? PushDuration::Long
                                                                                            : PushDuration::Short);
                }

                m_TimePushButtonPressedDown = 0;
            }
        }
    }

    // GPIO status
    if (getGPIOConfiguration(GPIOPin::GPIO1).PinMode == GPIOPinMode::DigitalInput &&
        getGPIOConfiguration(GPIOPin::GPIO1).InputEdgeCallback)
    {
        if (secondaryInterruptStatus.GPIO1_HadNegativeEdge)
        {
            getGPIOConfiguration(GPIOPin::GPIO1).InputEdgeCallback(GPIOInputEdge::Negative);
        }

        if (secondaryInterruptStatus.GPIO1_HadPositiveEdge)
        {
            getGPIOConfiguration(GPIOPin::GPIO1).InputEdgeCallback(GPIOInputEdge::Positive);
        }
    }

    if (getGPIOConfiguration(GPIOPin::GPIO2).PinMode == GPIOPinMode::DigitalInput &&
        getGPIOConfiguration(GPIOPin::GPIO2).InputEdgeCallback)
    {
        if (secondaryInterruptStatus.GPIO2_HadNegativeEdge)
        {
            getGPIOConfiguration(GPIOPin::GPIO2).InputEdgeCallback(GPIOInputEdge::Negative);
        }

        if (secondaryInterruptStatus.GPIO2_HadPositiveEdge)
        {
            getGPIOConfiguration(GPIOPin::GPIO2).InputEdgeCallback(GPIOInputEdge::Positive);
        }
    }
}

void Encoder::setEnabled(bool const fEnabled)
{
    if (fEnabled != m_fEnabled)
    {
        m_fEnabled = fEnabled;
        updateColor();
    }
}

void Encoder::setColor(RGBColor const& color)
{
    if (color != m_Color)
    {
        m_Color = color;
        updateColor();
    }
}

void Encoder::updateColor() const
{
    RGBColor const color = m_fEnabled ? m_Color : Colors::Disabled;

    AutoTransmission autoTransmission(m_Wire, m_Address);

    m_Wire.write(static_cast<uint8_t>(I2CRegister::LED_R));
    m_Wire.write(color.Red);
    m_Wire.write(color.Green);
    m_Wire.write(color.Blue);
}

void Encoder::setIncrementOrderOfMagnitude(uint8_t const incrementOrderOfMagnitude)
{
    m_IncrementOrderOfMagnitude = incrementOrderOfMagnitude;

    auto const powerOf10 = [](uint8_t const exponent) -> uint32_t {
        uint32_t value = 1;

        for (uint8_t idxExponent = 0; idxExponent < exponent; ++idxExponent)
        {
            value = value * 10;
        }

        return value;
    };

    writeRegister(I2CRegister::StepValue_0, powerOf10(m_IncrementOrderOfMagnitude));
}

uint8_t Encoder::getIncrementOrderOfMagnitude()
{
    return m_IncrementOrderOfMagnitude;
}

void Encoder::setGPIOOutput(GPIOPin const pin, uint8_t const value)
{
    switch (getGPIOConfiguration(pin).PinMode)
    {
        case GPIOPinMode::PushPullOutput:
        case GPIOPinMode::PWMOutput:
            writeRegister(getGPIOValueRegister(pin), value);
            break;

        default:
            return;
    }
}

uint8_t Encoder::getGPIOInput(GPIOPin const pin)
{
    switch (getGPIOConfiguration(pin).PinMode)
    {
        case GPIOPinMode::DigitalInput:
        case GPIOPinMode::AnalogInput:
            return readRegister<uint8_t>(getGPIOValueRegister(pin));

        default:
            return 0;
    }
}
