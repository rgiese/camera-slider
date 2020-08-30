#include "../inc/stdinc.h"

void MotorHomingReverseState::onEnteringState()
{
    setDisplay("Homing motor");

    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        g_StateKeeper.RequestState(new InitializingMotorState());
        return;
    }

    g_MotorController.goHomeReverse();
}

void MotorHomingReverseState::onLoop()
{
    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        g_StateKeeper.RequestState(new InitializingMotorState());
        return;
    }

    if (!g_MotorController.getHomingActive())
    {
        // Homing complete
        g_StateKeeper.RequestState(new MotorInitializedState());
    }
}