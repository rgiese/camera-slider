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
    m_Tic.setMaxSpeed(speedToTicUnits(stepsPerSecond));
}

void MotorController::goHome()
{
    m_Tic.goHomeReverse();
}