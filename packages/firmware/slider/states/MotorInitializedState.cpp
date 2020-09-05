#include "../inc/stdinc.h"
#include "states.h"

void MotorInitializedState::onEnteringState()
{
    Display::set("Initialized");
}

void MotorInitializedState::onLoop()
{
    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }
}

bool MotorInitializedState::onRequest(Request const& request)
{
    // TEMPORARY: Testing other states
    switch (request.Type)
    {
        case RequestType::UIButtonPressed:
            g_StateKeeper.RequestState(new MotorFindingForwardRangeState());
            return true;

        default:
            return false;
    }
}