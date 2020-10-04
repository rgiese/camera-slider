#include "../inc/stdinc.h"
#include "states.h"

using namespace Flatbuffers::Firmware;

void RunningMovementProgramState::onEnteringState()
{
    g_Display.set("Running program");

    m_spMovementProgram->dump();

    if (!m_spMovementProgram->get().movements()->size())
    {
        // Empty program -> exit state
        return exitProgram();
    }

    enterStep(0);
}

void RunningMovementProgramState::enterStep(size_t const idxStep)
{
    Movement const& movement = *m_spMovementProgram->get().movements()->Get(idxStep);

    // Prep for movement
    switch (movement.type())
    {
        case MovementType::Move: {
            // Apply desired motor settings
            double const rateMultiplier = m_spMovementProgram->get().rate() / 100.0f;

            g_MotorController.setMaxSpeed(static_cast<uint32_t>(movement.desiredSpeed() * rateMultiplier));
            g_MotorController.setMaxAcceleration(
                static_cast<uint32_t>(movement.desiredAcceleration() * rateMultiplier));

            // Seek to position
            g_MotorController.setTargetPosition(movement.desiredPosition());

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
    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        return g_StateKeeper.RequestState(new InitializingMotorState());
    }

    if (!g_MotorController.isPositionCertain())
    {
        return g_StateKeeper.RequestState(new HomingState());
    }

    // Check if movement is completed
    Movement const& movement = *m_spMovementProgram->get().movements()->Get(m_idxCurrentStep);

    switch (movement.type())
    {
        case MovementType::Move: {
            if (g_MotorController.getCurrentPosition() == movement.desiredPosition())
            {
                return nextStep();
            }
            break;
        }

        case MovementType::Delay: {
            unsigned long const timeDelayed_msec = millis() - m_DelayStart_msec;

            double const rateMultiplier = m_spMovementProgram->get().rate() / 100.0f;

            if (timeDelayed_msec > static_cast<unsigned long>(movement.delayTime() / rateMultiplier))
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
    size_t const cMovements = m_spMovementProgram->get().movements()->size();

    if (m_idxCurrentStep >= (cMovements - 1))
    {
        // Reached end of movement list
        bool const isRepeatRequested = !!(m_spMovementProgram->get().flags() & MovementProgramFlags::Repeat);

        if (isRepeatRequested && cMovements > 1)
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
            Serial.printlnf("!! Safety stop seek to %d", g_MotorController.getCurrentPosition());
            g_MotorController.safetyStop();
            g_StateKeeper.RequestState(new TrackingDesiredPositionState());
            return true;

        case RequestType::DesiredMovementProgram: {
            std::shared_ptr<MovementProgramOwner> updatedMovementProgram =
                request.DesiredMovementProgram.MovementProgram;

            bool const shouldAdoptInPlace =
                m_spMovementProgram->get().movements()->size() <= updatedMovementProgram->get().movements()->size();

            m_spMovementProgram = updatedMovementProgram;

            if (shouldAdoptInPlace)
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