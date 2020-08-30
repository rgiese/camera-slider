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
        g_StateKeeper.RequestState(new InitializingMotorState());
        return;
    }
}