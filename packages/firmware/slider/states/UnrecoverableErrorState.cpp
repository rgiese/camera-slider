#include "../inc/stdinc.h"
#include "states.h"

UnrecoverableErrorState::UnrecoverableErrorState(char const* format, ...)
{
    va_list args;
    va_start(args, format);

    vsnprintf(m_ErrorMessage, countof(m_ErrorMessage), format, args);
    m_ErrorMessage[countof(m_ErrorMessage) - 1] = 0;

    va_end(args);
}

void UnrecoverableErrorState::onEnteringState()
{
    Display::set(m_ErrorMessage);
}

void UnrecoverableErrorState::onLoop()
{
    Serial.print("Unrecoverable error: ");
    Serial.println(m_ErrorMessage);

    delay(1000);
}
