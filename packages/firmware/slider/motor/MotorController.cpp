#include "inc/stdinc.h"

MotorController g_MotorController;

//
// Main functions
//

void MotorController::begin()
{
    delay(100);  // Give motor controller time to start
    m_Tic.setProduct(TicProduct::T500);
}

void MotorController::onLoop()
{
    // Reset watchdog on motor controller
    m_Tic.resetCommandTimeout();

    // Refresh latest values
    m_OperationState = m_Tic.getOperationState();
    m_CurrentPosition = m_Tic.getCurrentPosition();
    m_IsPositionCertain = !m_Tic.getPositionUncertain();
}

//
// Setters
//

void MotorController::setTargetPosition(int32_t const targetPosition)
{
    m_Tic.setTargetPosition(targetPosition);
}

void MotorController::setMaxSpeed(uint32_t const stepsPerSecond)
{
    m_Tic.setMaxSpeed(speedToTicUnits(std::min(stepsPerSecond, c_MaxSafeSpeed_StepsPerSec)));
}

void MotorController::setMaxAcceleration(uint32_t const stepsPerSecondPerSecond)
{
    m_Tic.setMaxAccel(
        accelerationToTicUnits(std::min(stepsPerSecondPerSecond, c_MaxSafeAcceleration_StepsPerSecPerSec)));
    m_Tic.setMaxDecel(
        accelerationToTicUnits(std::min(stepsPerSecondPerSecond, c_MaxSafeAcceleration_StepsPerSecPerSec)));
}

void MotorController::goHome()
{
    m_Tic.goHomeReverse();
}

void MotorController::safetyStop()
{
    // Save current values
    int32_t const desiredPosition = m_Tic.getCurrentPosition();
    uint32_t const desiredMaxAcceleration = getMaximumAcceleration();

    // Apply safety stop
    setMaxAcceleration(MotorController::c_MaxSafeAcceleration_StepsPerSecPerSec);
    setTargetPosition(desiredPosition);

    // Wait for safety stop to complete
    while (m_Tic.getCurrentPosition() != desiredPosition)
    {
        if (m_Tic.getPositionUncertain())
        {
            // Stop waiting; let someone else deal with it
            break;
        }
    }

    // Restore previous values
    setMaxAcceleration(desiredMaxAcceleration);
}