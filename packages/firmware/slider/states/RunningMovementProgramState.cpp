#include "../inc/stdinc.h"
#include "states.h"

void RunningMovementProgramState::onEnteringState()
{
    g_Display.set("Running movement program");

    m_spMovementProgram->dump();
}

void RunningMovementProgramState::onLoop()
{
    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    if (!g_MotorController.isPositionCertain())
    {
        return g_StateKeeper.RequestState(new HomingState());
    }
}

bool RunningMovementProgramState::onRequest(Request const& request)
{
    switch (request.Type)
    {
        case RequestType::UIButtonPressed:
            // Safety stop, return to tracking control
            g_MotorController.setTargetPosition(g_MotorController.getCurrentPosition());
            g_StateKeeper.RequestState(new TrackingDesiredPositionState());
            return true;

        case RequestType::DesiredMovementProgram:
            // TODO: Adopt updated movement program
            return true;

        default:
            return false;
    }
}