#include "../inc/stdinc.h"

void HomingState::onEnteringState()
{
    g_Display.set("Homing");

    if (g_MotorController.OperationState != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    g_MotorController.setMaxSpeed(MotorController::c_MaxSafeHomingSpeed_StepsPerSec);
    g_MotorController.goHome();
}

void HomingState::onLoop()
{
    if (g_MotorController.OperationState != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    if (!g_MotorController.isHomingActive())
    {
        // Homing complete
        return g_StateKeeper.RequestState(new TrackingDesiredPositionState());
    }
}