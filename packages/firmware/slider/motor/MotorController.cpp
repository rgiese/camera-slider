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

    // Refresh observables
    OperationState.update(m_Tic.getOperationState());
    IsPositionCertain.update(!m_Tic.getPositionUncertain());
    CurrentPosition.update(m_Tic.getCurrentPosition());
    CurrentVelocity.update(velocityFromTicUnits(m_Tic.getCurrentVelocity()));
    MaximumSpeed.update(speedFromTicUnits(m_Tic.getMaxSpeed()));
    MaximumAcceleration.update(accelerationFromTicUnits(m_Tic.getMaxAccel()));
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
    uint32_t const safeStepsPerSecondPerSecond =
        std::min(stepsPerSecondPerSecond, c_MaxSafeAcceleration_StepsPerSecPerSec);

    uint32_t const ticStepsPerSecondPerSecond = accelerationToTicUnits(safeStepsPerSecondPerSecond);

    m_Tic.setMaxAccel(ticStepsPerSecondPerSecond);
    m_Tic.setMaxDecel(ticStepsPerSecondPerSecond);
}

void MotorController::goHome()
{
    m_Tic.goHomeReverse();
}

void MotorController::safetyStop()
{
    // Save current values
    int32_t const desiredPosition = m_Tic.getCurrentPosition();
    uint32_t const desiredMaxAcceleration = MaximumAcceleration;

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