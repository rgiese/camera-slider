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
    TargetPosition.update(m_Tic.getTargetPosition());
    MaximumSpeed.update(speedFromTicUnits(m_Tic.getMaxSpeed()));
    MaximumAcceleration.update(accelerationFromTicUnits(m_Tic.getMaxAccel()));
    MaximumDeceleration.update(accelerationFromTicUnits(m_Tic.getMaxDecel()));
}

//
// Setters
//

void MotorController::setTargetPosition(int32_t const targetPosition)
{
    m_Tic.setTargetPosition(clamp<int32_t>(targetPosition, c_MinimumPosition_Steps, c_MaxSafePosition_Steps));
}

void MotorController::setMaxSpeed(uint32_t const stepsPerSecond)
{
    m_Tic.setMaxSpeed(speedToTicUnits(clamp(stepsPerSecond, c_MinimumSpeed_StepsPerSec, c_MaxSafeSpeed_StepsPerSec)));
}

void MotorController::setMaxAcceleration(uint32_t const stepsPerSecondPerSecond)
{
    uint32_t const safeStepsPerSecondPerSecond = clamp(
        stepsPerSecondPerSecond, c_MinimumAcceleration_StepsPerSecPerSec, c_MaxSafeAcceleration_StepsPerSecPerSec);

    uint32_t const ticStepsPerSecondPerSecond = accelerationToTicUnits(safeStepsPerSecondPerSecond);

    m_Tic.setMaxAccel(ticStepsPerSecondPerSecond);
}

void MotorController::setMaxDeceleration(uint32_t const stepsPerSecondPerSecond)
{
    uint32_t const safeStepsPerSecondPerSecond = clamp(
        stepsPerSecondPerSecond, c_MinimumAcceleration_StepsPerSecPerSec, c_MaxSafeAcceleration_StepsPerSecPerSec);

    uint32_t const ticStepsPerSecondPerSecond = accelerationToTicUnits(safeStepsPerSecondPerSecond);

    m_Tic.setMaxDecel(ticStepsPerSecondPerSecond);
}

void MotorController::goHome()
{
    m_Tic.setTargetPosition(0);  // Update internal tracking
    m_Tic.goHomeReverse();
}

void MotorController::safetyStop()
{
    // Save current values
    int32_t const desiredPosition = m_Tic.getCurrentPosition();
    uint32_t const desiredMaxAcceleration = MaximumAcceleration;
    uint32_t const desiredMaxDeceleration = MaximumDeceleration;

    // Apply safety stop
    setMaxAcceleration(MotorController::c_MaxSafeAcceleration_StepsPerSecPerSec);
    setMaxDeceleration(MotorController::c_MaxSafeAcceleration_StepsPerSecPerSec);
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
    setMaxDeceleration(desiredMaxDeceleration);
}