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
            // TEMPORARY: Testing
            g_StateKeeper.RequestState(new FindingForwardRangeState());
            return true;

        case RequestType::DesiredPosition:
            g_MotorController.setTargetPosition(request.Data.DesiredPosition);
            return true;

        default:
            return false;
    }
}