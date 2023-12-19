#pragma once

class RawTouchPoint;

class CalibrateTouchScreenState : public AbstractState
{
public:
    struct TouchScreenCalibrationData
    {
        Touch::RawTouchPoint None;
        Touch::RawTouchPoint TopLeft;
        Touch::RawTouchPoint TopRight;
        Touch::RawTouchPoint BottomLeft;
        Touch::RawTouchPoint BottomRight;
    };

public:
    CalibrateTouchScreenState() : AbstractState(SliderState::CalibrateTouchStart), m_CalibrationData()
    {
    }

    CalibrateTouchScreenState(SliderState const sliderState, TouchScreenCalibrationData const& calibrationData)
        : AbstractState(sliderState), m_CalibrationData(calibrationData)
    {
    }

    bool onRequest(Request const& request) override;

private:
    TouchScreenCalibrationData const m_CalibrationData;
};
