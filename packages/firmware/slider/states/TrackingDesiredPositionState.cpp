#include "../inc/stdinc.h"
#include "states.h"

void TrackingDesiredPositionState::onEnteringState()
{
    g_Display.set("Tracking desired position");
}

void TrackingDesiredPositionState::onLoop()
{
    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    if (!g_MotorController.isPositionCertain())
    {
        return g_StateKeeper.RequestState(new HomingState());
    }
}

bool TrackingDesiredPositionState::onRequest(Request const& request)
{
    switch (request.Type)
    {
        case RequestType::UIButtonPressed:
            // Safety return
            g_MotorController.safetyStop();
            g_MotorController.setTargetPosition(0);
            return true;

        case RequestType::DesiredPosition:
            g_MotorController.setTargetPosition(request.DesiredPosition.value);
            return true;

        case RequestType::DesiredMaximumSpeed:
            g_MotorController.setMaxSpeed(request.DesiredMaximumSpeed.value);
            return true;

        case RequestType::DesiredMaximumAcceleration:
            g_MotorController.setMaxAcceleration(request.DesiredMaximumAcceleration.value);
            return true;

        case RequestType::StartMovementProgram:
            g_StateKeeper.RequestState(new RunningMovementProgramState(request.StartMovementProgram.atStep));
            return true;

        default:
            return false;
    }
}