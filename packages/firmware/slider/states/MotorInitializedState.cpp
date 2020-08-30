#include "../inc/stdinc.h"
#include "states.h"

void MotorInitializedState::onEnteringState()
{
    setDisplay("Initialized");
}

void MotorInitializedState::onLoop()
{
    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }
}

void MotorInitializedState::onUIButtonPressed()
{
    // TEMPORARY: Testing other states
    g_StateKeeper.RequestState(new MotorFindingForwardRangeState());
}