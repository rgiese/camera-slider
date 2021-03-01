#include "../inc/stdinc.h"
#include "states.h"

bool CalibrateTouchScreenState::onRequest(Request const& request)
{
    if (request.Type != RequestType::CalibrateTouchNext)
    {
        return false;
    }

    Touch::RawTouchPoint const rawTouchPoint = Touch::getRawTouchPoint();

    TouchScreenCalibrationData calibrationData = m_CalibrationData;

    switch (getSliderState())
    {
        case SliderState::CalibrateTouchStart:
            calibrationData.None = rawTouchPoint;
            g_StateKeeper.RequestState(
                new CalibrateTouchScreenState(SliderState::CalibrateTouchTopLeft, calibrationData));
            return true;

        case SliderState::CalibrateTouchTopLeft:
            calibrationData.TopLeft = rawTouchPoint;
            g_StateKeeper.RequestState(
                new CalibrateTouchScreenState(SliderState::CalibrateTouchTopRight, calibrationData));
            return true;

        case SliderState::CalibrateTouchTopRight:
            calibrationData.TopRight = rawTouchPoint;
            g_StateKeeper.RequestState(
                new CalibrateTouchScreenState(SliderState::CalibrateTouchBottomLeft, calibrationData));
            return true;

        case SliderState::CalibrateTouchBottomLeft:
            calibrationData.BottomLeft = rawTouchPoint;
            g_StateKeeper.RequestState(
                new CalibrateTouchScreenState(SliderState::CalibrateTouchBottomRight, calibrationData));
            return true;

        case SliderState::CalibrateTouchBottomRight:
            calibrationData.BottomRight = rawTouchPoint;
            {
                Serial.println("Touch calibration points:");

                auto const printPoint = [](Touch::RawTouchPoint const& point, char const* label) {
                    Serial.printlnf("  %s: %f / %f / %f", label, point.X, point.Y, point.Z);
                };

                printPoint(calibrationData.None, "None");
                printPoint(calibrationData.TopLeft, "Top left");
                printPoint(calibrationData.TopRight, "Top right");
                printPoint(calibrationData.BottomLeft, "Bottom left");
                printPoint(calibrationData.BottomRight, "Bottom right");
            }

            g_StateKeeper.RequestState(new TrackingDesiredPositionState());
            return true;
    }

    return false;
}