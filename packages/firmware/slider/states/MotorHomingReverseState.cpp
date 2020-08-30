#include "../inc/stdinc.h"

void MotorHomingReverseState::onEnteringState()
{
    Display::set("Homing motor");

    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    g_MotorController.setMaxSpeed(TicTools::Speed::toTicUnits(TicTools::Speed::c_MaxSafeHomingSpeed_StepsPerSec));
    g_MotorController.goHomeReverse();
}

void MotorHomingReverseState::onLoop()
{
    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    if (!g_MotorController.getHomingActive())
    {
        // Homing complete
        return g_StateKeeper.RequestState(new MotorInitializedState());
    }
}