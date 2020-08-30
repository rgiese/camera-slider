#include "../inc/stdinc.h"
#include "states.h"

void UnrecoverableErrorState::onEnteringState()
{
    setDisplay("Error");
}

void UnrecoverableErrorState::onLoop()
{
    Serial.println("Unrecoverable error");
    delay(1000);
}
