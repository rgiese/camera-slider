#include "../inc/stdinc.h"

void HomingState::onEnteringState()
{
    g_Display.set("Homing");

    if (g_MotorController.OperationState != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    m_PreviousMaxSpeed_StepsPerSec = g_MotorController.MaximumSpeed.get();

    g_MotorController.setMaxSpeed(MotorController::c_DefaultHomingSpeed_StepsPerSec);
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
        g_MotorController.setMaxSpeed(m_PreviousMaxSpeed_StepsPerSec);
        return g_StateKeeper.RequestState(new TrackingDesiredPositionState());
    }
}