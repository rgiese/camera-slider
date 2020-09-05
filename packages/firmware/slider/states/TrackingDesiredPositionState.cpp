#include "../inc/stdinc.h"
#include "states.h"

void TrackingDesiredPositionState::onEnteringState()
{
    Display::set("Tracking desired position");
}

void TrackingDesiredPositionState::onLoop()
{
    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }
}

bool TrackingDesiredPositionState::onRequest(Request const& request)
{
    // TEMPORARY: Testing other states
    switch (request.Type)
    {
        case RequestType::UIButtonPressed:
            g_StateKeeper.RequestState(new FindingForwardRangeState());
            return true;

        default:
            return false;
    }
}