#include "../inc/stdinc.h"
#include "states.h"

void InitializingMotorState::onEnteringState()
{
    g_Display.set("Initializing motor");
}

void InitializingMotorState::onLoop()
{
    if (g_MotorController.tryInitialize())
    {
        return g_StateKeeper.RequestState(new HomingState());
    }
}
