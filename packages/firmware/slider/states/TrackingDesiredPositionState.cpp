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

        case RequestType::DesiredParameterDelta_Position:
            g_MotorController.setTargetPosition(g_MotorController.TargetPosition + request.DesiredParameterDelta.delta);
            return true;

        case RequestType::DesiredParameterDelta_MaximumSpeed:
            g_MotorController.setMaxSpeed(g_MotorController.MaximumSpeed + request.DesiredParameterDelta.delta);
            return true;

        case RequestType::DesiredParameterDelta_MaximumAcceleration:
            g_MotorController.setMaxAcceleration(g_MotorController.MaximumAcceleration +
                                                 request.DesiredParameterDelta.delta);
            return true;

        case RequestType::StartMovementProgram:
            g_StateKeeper.RequestState(new RunningMovementProgramState(request.StartMovementProgram.atStep));
            return true;

        default:
            return false;
    }
}