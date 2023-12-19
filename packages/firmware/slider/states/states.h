#pragma once

//
// Our state machine is articulated through transient objects derived from StateRoot.
// The lifetime of each state object is bound to the lifetime of the given state.
//
// The StateKeeper will transition state objects on loop ticks only
// so we can make sure only one state object is operating at a time.
//

enum class SliderState
{
    FindingForwardRange,
    Homing,
    InitializingMotor,
    RunningMovementProgram,
    TrackingDesiredPosition,
    UnrecoverableError,
    CalibrateTouchStart,
    CalibrateTouchTopLeft,
    CalibrateTouchTopRight,
    CalibrateTouchBottomLeft,
    CalibrateTouchBottomRight,
};

inline char const* getSliderStateName(SliderState const state)
{
    switch (state)
    {
        case SliderState::FindingForwardRange:
            return "FindingForwardRange";
        case SliderState::Homing:
            return "Homing";
        case SliderState::InitializingMotor:
            return "InitializingMotor";
        case SliderState::RunningMovementProgram:
            return "RunningMovementProgram";
        case SliderState::TrackingDesiredPosition:
            return "TrackingDesiredPosition";
        case SliderState::UnrecoverableError:
            return "UnrecoverableError";
        case SliderState::CalibrateTouchStart:
            return "CalibrateTouchStart";
        case SliderState::CalibrateTouchTopLeft:
            return "CalibrateTouchTopLeft";
        case SliderState::CalibrateTouchTopRight:
            return "CalibrateTouchTopRight";
        case SliderState::CalibrateTouchBottomLeft:
            return "CalibrateTouchBottomLeft";
        case SliderState::CalibrateTouchBottomRight:
            return "CalibrateTouchBottomRight";
        default:
            return "<unknown>";
    }
}

inline char const* getSliderStateFriendlyName(SliderState const state)
{
    switch (state)
    {
        case SliderState::FindingForwardRange:
            return "Finding forward range";
        case SliderState::Homing:
            return "Homing";
        case SliderState::InitializingMotor:
            return "Initializing motor";
        case SliderState::RunningMovementProgram:
            return "Running movement program";
        case SliderState::TrackingDesiredPosition:
            return "Tracking desired position";
        case SliderState::UnrecoverableError:
            return "Unrecoverable error";
        case SliderState::CalibrateTouchStart:
            return "Calibrate touch start";
        case SliderState::CalibrateTouchTopLeft:
            return "Calibrate touch top left";
        case SliderState::CalibrateTouchTopRight:
            return "Calibrate touch top right";
        case SliderState::CalibrateTouchBottomLeft:
            return "Calibrate touch bottom left";
        case SliderState::CalibrateTouchBottomRight:
            return "Calibrate touch bottom right";
        default:
            return "<unknown>";
    }
}

#include "AbstractState.h"
#include "StateKeeper.h"

extern StateKeeper g_StateKeeper;

#include "UnrecoverableErrorState.h"

#include "InitializingMotorState.h"
#include "HomingState.h"
#include "RunningMovementProgramState.h"
#include "TrackingDesiredPositionState.h"
#include "FindingForwardRangeState.h"
#include "CalibrateTouchScreenState.h"