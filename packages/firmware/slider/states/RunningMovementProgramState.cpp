#include "../inc/stdinc.h"
#include "states.h"

void RunningMovementProgramState::onEnteringState()
{
    g_Display.set("Running program");

    m_CurrentMovementProgram = g_MovementProgramStore.CurrentMovementProgram;
    m_CurrentMovementProgram.dump("running");

    if (!m_CurrentMovementProgram.Movements.size())
    {
        // Empty program -> exit state
        return exitProgram();
    }

    enterStep(m_idxInitialStep < m_CurrentMovementProgram.Movements.size() ? m_idxInitialStep : 0);
}

void RunningMovementProgramState::enterStep(size_t const idxStep)
{
    MovementProgram::Movement const& movement = m_CurrentMovementProgram.Movements.at(idxStep);

    // Prep for movement
    switch (movement.Type)
    {
        case MovementType::Move: {
            // Apply desired motor settings
            g_MotorController.setMaxSpeed(
                static_cast<uint32_t>(movement.DesiredSpeed * m_CurrentMovementProgram.RatePercent / 100.0f));
            g_MotorController.setMaxAcceleration(
                static_cast<uint32_t>(movement.DesiredAcceleration * m_CurrentMovementProgram.RatePercent / 100.0f));
            g_MotorController.setMaxDeceleration(
                static_cast<uint32_t>(movement.DesiredDeceleration * m_CurrentMovementProgram.RatePercent / 100.0f));

            // Seek to position
            g_MotorController.setTargetPosition(movement.DesiredPosition);

            break;
        }

        case MovementType::Delay: {
            // Snap current time
            m_DelayStart_msec = millis();
            break;
        }

        default:
            // Don't know what to do, abandon
            return exitProgram();
    }

    // Commit movement
    m_idxCurrentStep = idxStep;
}

void RunningMovementProgramState::onLoop()
{
    // Motor checks
    if (g_MotorController.OperationState != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    if (!g_MotorController.IsPositionCertain)
    {
        return g_StateKeeper.RequestState(new HomingState());
    }

    // Check if movement is completed
    MovementProgram::Movement const& movement = m_CurrentMovementProgram.Movements.at(m_idxCurrentStep);

    switch (movement.Type)
    {
        case MovementType::Move: {
            if (g_MotorController.CurrentPosition == movement.DesiredPosition)
            {
                return nextStep();
            }
            break;
        }

        case MovementType::Delay: {
            unsigned long const timeDelayed_msec = millis() - m_DelayStart_msec;

            if (timeDelayed_msec >
                static_cast<unsigned long>(movement.DelayTime * m_CurrentMovementProgram.RatePercent / 100.0f))
            {
                m_DelayStart_msec = 0;
                return nextStep();
            }
            break;
        }

        default:
            // Shouldn't have gotten here, abandon
            return exitProgram();
    }
}

void RunningMovementProgramState::nextStep()
{
    if ((m_idxCurrentStep + 1) >= m_CurrentMovementProgram.Movements.size())
    {
        // Reached end of movement list
        if (m_CurrentMovementProgram.Repeat && m_CurrentMovementProgram.Movements.size() > 1)
        {
            // Repeat (provided we had more than one step)
            return enterStep(0);
        }
        else
        {
            // Done
            return exitProgram();
        }
    }
    else
    {
        // Next step
        enterStep(m_idxCurrentStep + 1);
    }
}

void RunningMovementProgramState::exitProgram()
{
    g_StateKeeper.RequestState(new TrackingDesiredPositionState());
}

bool RunningMovementProgramState::onRequest(Request const& request)
{
    switch (request.Type)
    {
        case RequestType::StopMovementProgram:
            g_MotorController.safetyStop();
            g_StateKeeper.RequestState(new TrackingDesiredPositionState());
            return true;

        case RequestType::StartMovementProgram:
            if (request.StartMovementProgram.atStep != m_idxCurrentStep &&
                request.StartMovementProgram.atStep < m_CurrentMovementProgram.Movements.size())
            {
                enterStep(request.StartMovementProgram.atStep);
            }
            return true;

        case RequestType::UpdatedMovementProgram:
            m_CurrentMovementProgram = g_MovementProgramStore.CurrentMovementProgram;

            if (m_idxCurrentStep < m_CurrentMovementProgram.Movements.size())
            {
                // Reboot step to update
                enterStep(m_idxCurrentStep);
            }
            else if (!m_CurrentMovementProgram.Movements.empty())
            {
                // Reboot entire program
                enterStep(0);
            }
            else
            {
                // Nothing left to execute
                exitProgram();
            }
            return true;

        default:
            return false;
    }
}