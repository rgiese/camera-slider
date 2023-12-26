#include "../inc/stdinc.h"
#include "states.h"

void TrackingDesiredPositionState::onEnteringState()
{
    g_Display.set("Tracking desired position");
    g_MotorController.setTargetPosition(g_MotorController.CurrentPosition);
}

void TrackingDesiredPositionState::onLoop()
{
    if (g_MotorController.OperationState != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    if (!g_MotorController.IsPositionCertain)
    {
        return g_StateKeeper.RequestState(new HomingState());
    }
}

bool TrackingDesiredPositionState::onRequest(Request const& request)
{
    switch (request.Type)
    {
        case RequestType::DesiredPosition:
            g_MotorController.setTargetPosition(request.DesiredPosition.value);
            return true;

        case RequestType::DesiredMaximumSpeed:
            g_MotorController.setMaxSpeed(request.DesiredMaximumSpeed.value);
            return true;

        case RequestType::DesiredMaximumAcceleration:
            g_MotorController.setMaxAcceleration(request.DesiredMaximumAcceleration.value);
            return true;

        case RequestType::DesiredMaximumDeceleration:
            g_MotorController.setMaxDeceleration(request.DesiredMaximumDeceleration.value);
            return true;

        case RequestType::DesiredParameterDelta_Position:
            g_MotorController.setTargetPosition(clamp_delta(g_MotorController.TargetPosition.get(),
                                                            request.DesiredParameterDelta.delta,
                                                            MotorController::c_MinimumPosition_Steps,
                                                            MotorController::c_MaxSafePosition_Steps));
            return true;

        case RequestType::DesiredParameterDelta_MaximumSpeed:
            g_MotorController.setMaxSpeed(clamp_delta(g_MotorController.MaximumSpeed.get(),
                                                      request.DesiredParameterDelta.delta,
                                                      MotorController::c_MinimumSpeed_StepsPerSec,
                                                      MotorController::c_MaxSafeSpeed_StepsPerSec));
            return true;

        case RequestType::DesiredParameterDelta_MaximumAcceleration:
            g_MotorController.setMaxAcceleration(clamp_delta(g_MotorController.MaximumAcceleration.get(),
                                                             request.DesiredParameterDelta.delta,
                                                             MotorController::c_MinimumAcceleration_StepsPerSecPerSec,
                                                             MotorController::c_MaxSafeAcceleration_StepsPerSecPerSec));
            return true;

        case RequestType::DesiredParameterDelta_MaximumDeceleration:
            g_MotorController.setMaxDeceleration(clamp_delta(g_MotorController.MaximumDeceleration.get(),
                                                             request.DesiredParameterDelta.delta,
                                                             MotorController::c_MinimumAcceleration_StepsPerSecPerSec,
                                                             MotorController::c_MaxSafeAcceleration_StepsPerSecPerSec));
            return true;

        case RequestType::StartMovementProgram:
            g_StateKeeper.RequestState(new RunningMovementProgramState(request.StartMovementProgram.atStep));
            return true;

        case RequestType::CalibrateTouchStart:
            g_StateKeeper.RequestState(new CalibrateTouchScreenState());
            return true;

        default:
            return false;
    }
}