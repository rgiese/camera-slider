#include "../inc/stdinc.h"
#include "states.h"

void InitializingMotorState::onEnteringState()
{
    setDisplay("Initializing motor");
}

void InitializingMotorState::onLoop()
{
    // De-energize to make sure communications with motor controller are working correctly
    g_MotorController.exitSafeStart();
    g_MotorController.deenergize();

    if (g_MotorController.getOperationState() != TicOperationState::Deenergized)
    {
        // Retry on next tick
        return;
    }

    // Validate burned-in motor controller settings
    {
        uint16_t commandTimeout = 0;

        g_MotorController.getSetting(0x09, sizeof(commandTimeout), reinterpret_cast<uint8_t*>(&commandTimeout));

        if (commandTimeout > 1000 /* msec */)
        {
            // Unsafe command timeout
            g_StateKeeper.RequestState(new UnrecoverableErrorState());
            return;
        }
    }

    // Energize
    g_MotorController.exitSafeStart();
    g_MotorController.energize();

    if (g_MotorController.getOperationState() != TicOperationState::Normal)
    {
        // Retry on next tick
        return;
    }

    g_MotorController.exitSafeStart();

    // Advance state
    g_StateKeeper.RequestState(new MotorHomingReverseState());
}
