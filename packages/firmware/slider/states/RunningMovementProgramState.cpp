#include "../inc/stdinc.h"
#include "states.h"

void RunningMovementProgramState::onEnteringState()
{
    g_Display.set("Running program");

    MovementProgram const& movementProgram = g_MovementProgramStore.getMovementProgram();
    movementProgram.dump();

    if (!movementProgram.Movements.size())
    {
        // Empty program -> exit state
        return exitProgram();
    }

    enterStep(0);
}

void RunningMovementProgramState::enterStep(size_t const idxStep)
{
    MovementProgram const& movementProgram = g_MovementProgramStore.getMovementProgram();
    MovementProgram::Movement const& movement = movementProgram.Movements.at(idxStep);

    // Prep for movement
    switch (movement.Type)
    {
        case Flatbuffers::Firmware::MovementType::Move: {
            // Apply desired motor settings
            g_MotorController.setMaxSpeed(static_cast<uint32_t>(movement.DesiredSpeed * movementProgram.Rate));
            g_MotorController.setMaxAcceleration(
                static_cast<uint32_t>(movement.DesiredAcceleration * movementProgram.Rate));

            // Seek to position
            g_MotorController.setTargetPosition(movement.DesiredPosition);

            break;
        }

        case Flatbuffers::Firmware::MovementType::Delay: {
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
    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    if (!g_MotorController.isPositionCertain())
    {
        return g_StateKeeper.RequestState(new HomingState());
    }

    // Check if movement is completed
    MovementProgram const& movementProgram = g_MovementProgramStore.getMovementProgram();
    MovementProgram::Movement const& movement = movementProgram.Movements.at(m_idxCurrentStep);

    switch (movement.Type)
    {
        case Flatbuffers::Firmware::MovementType::Move: {
            if (g_MotorController.getCurrentPosition() == movement.DesiredPosition)
            {
                return nextStep();
            }
            break;
        }

        case Flatbuffers::Firmware::MovementType::Delay: {
            unsigned long const timeDelayed_msec = millis() - m_DelayStart_msec;

            if (timeDelayed_msec > static_cast<unsigned long>(movement.DelayTime * movementProgram.Rate))
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
    MovementProgram const& movementProgram = g_MovementProgramStore.getMovementProgram();

    if ((m_idxCurrentStep + 1) >= movementProgram.Movements.size())
    {
        // Reached end of movement list
        bool const isRepeatRequested = !!(movementProgram.Flags & Flatbuffers::Firmware::MovementProgramFlags::Repeat);

        if (isRepeatRequested && movementProgram.Movements.size() > 1)
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
        case RequestType::UIButtonPressed:
        case RequestType::StopMovementProgram:
            // Safety stop, return to tracking control
            Serial.printlnf("!! Safety stop seek to %ld", g_MotorController.getCurrentPosition());
            g_MotorController.safetyStop();
            g_StateKeeper.RequestState(new TrackingDesiredPositionState());
            return true;

        case RequestType::UpdatedMovementProgram: {
            bool const canAdoptInPlace =
                m_idxCurrentStep < g_MovementProgramStore.getMovementProgram().Movements.size();

            if (canAdoptInPlace)
            {
                // Reboot step to update
                enterStep(m_idxCurrentStep);
            }
            else
            {
                // Reboot entire program
                onEnteringState();
            }

            return true;
        }

        default:
            return false;
    }
}