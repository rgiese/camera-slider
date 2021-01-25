#include "../inc/stdinc.h"

void FindingForwardRangeState::onEnteringState()
{
    g_Display.set("Finding forward range");

    // Make sure motor is ready and at a known position
    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    if (!g_MotorController.isPositionCertain())
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    // Set up slow homing speed since this is a bit of a janky process
    g_MotorController.setMaxSpeed(MotorController::c_MaxSafeHomingSpeed_StepsPerSec / 2);

    // Save position
    m_LatestCertainPosition = g_MotorController.getCurrentPosition();

    // Move as forward as we could possibly go
    g_MotorController.setTargetPosition(10000000);
}

void FindingForwardRangeState::onLoop()
{
    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        Serial.println("!! Motor controller exited normal state - abandoning forward range finding");

        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    if (!g_MotorController.isPositionCertain())
    {
        // Position now uncertain -> report latest certain position as max range
        Serial.printlnf("!! Max forward position: %ld", m_LatestCertainPosition);

        // Re-home motor reverse
        return g_StateKeeper.RequestState(new HomingState());
    }

    // Save latest position
    m_LatestCertainPosition = g_MotorController.getCurrentPosition();
}